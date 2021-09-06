#include <future>
#include <type_traits>
#include <typeinfo>
#include "game.h"
#include "snake.h"
#include "rival.h"


template <typename T>
T MessageQueue<T>::receive()
{

  std::unique_lock<std::mutex> Lock(_mutex);
  if(_queue.empty()) return NULL;
  T receivedMessage = std::move(_queue.back());
  _queue.clear();
  return receivedMessage;
}

template <typename T>
void MessageQueue<T>::send(T&& msg)
{
  std::lock_guard<std::mutex> Lock(_mutex);
  _queue.push_back(std::move(msg));
  _condition.notify_one();
}

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      grid_width(grid_width),
      grid_height(grid_height) { Initialize(); }

void Game::Initialize() {
  map = std::make_shared<Map<GameObject>>(grid_height, grid_width);
  player = std::make_shared<PlayerSnake>(grid_width, grid_height, map);
  player->Initialize();
  objectPool.push_back(player);
  Place<Food>();
  Place<RivalSnake>();
}

Game::~Game() {
    player = nullptr;
    food = nullptr;
    for (auto& gameobject : objectPool) {
        gameobject = nullptr;
    }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, player);
    Update();
    // map->print(); //  print to debug
    renderer.Render(objectPool);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

template <class T>
void Game::Place() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);

    // Check that the location is not occupied by a snake item before placing
    // food.
    if (map->at(y, x) != nullptr) continue;

    // Reassignment
    for (auto &gameobject : objectPool) {
      if (gameobject->isA<T>()) {
        if constexpr(std::is_same_v<T, Food>) {
          gameobject.reset(new T(x, y, map));
          food = std::dynamic_pointer_cast<Food>(gameobject);
          food->Initialize(score, x, y);
          return;
        }
        else if constexpr(std::is_same_v<T, RivalSnake>) {
        int obj_x = static_cast<int>(gameobject->origin_x);
        int obj_y = static_cast<int>(gameobject->origin_y);
        (*map)[obj_y][obj_x] = nullptr;
        gameobject.reset(new T(grid_width, grid_height, map));
        rival = std::dynamic_pointer_cast<T>(gameobject);
        rival->Initialize(score, x, y);
        return;
        }
     }
   }

   // Initialization
   if constexpr(std::is_same_v<T, Food>) {
     food = std::make_shared<T>(x, y, map);
     food->Initialize(0, x, y);
     objectPool.push_back(food);
   } else if constexpr(std::is_same_v<T, RivalSnake>){
     rival = std::make_shared<T>(grid_width, grid_height, map);
     rival->Initialize();
     objectPool.push_back(rival);
   }
   return;
 }
}

void Game::Update() {

  if(!player->alive) return;
  if(!rival->alive) Place<RivalSnake>();

  int randomInt = random_w(engine);
  std::thread foodConsumptionThread;
  std::promise<std::shared_ptr<Snake>> prmFoodConsumption;
  std::future<std::shared_ptr<Snake>> ftrFoodConsumption = prmFoodConsumption.get_future();

  clearMap();


  // Update and check for `foodConsumption` event
  for (auto& gameobject : objectPool) {
    gameobject->Update();
    if(gameobject->isA<Snake>()) {
      std::shared_ptr<Snake> snake = std::dynamic_pointer_cast<Snake>(gameobject);
      snake->setRandomInt(randomInt);
      if (snake->Collide(std::move(food.get()))) {
        foodConsumptionThread = std::thread(&Snake::Consume, snake, food,
          std::move(prmFoodConsumption));
      }
    }
  }


  // Send future to queue
  if (foodConsumptionThread.joinable()) {
    foodConsumptionThread.join();
    foodConsumptionQueue.send(std::move(ftrFoodConsumption.get()));
    Place<Food>();
  }

  CheckEvents();

}


void Game::CheckEvents() {

    // Food Consumption, receive future
    auto foodConsumer = foodConsumptionQueue.receive();
    if (foodConsumer != nullptr) {
      if (foodConsumer->isA<PlayerSnake>()) {
        if (foodConsumer->getFood()->getState() != Food::State::kPoison)
         score++;
       }
    }

    // Check on Player collides Rival
    if (player->Collide(std::move(rival.get()))) {
      rival->Shrink();
      if (!rival->alive) {
          Place<RivalSnake>();
          score++;
      }
      return;
    }

    // Check for Bullet collision
    if (!player->bullets.empty()) {
      for (auto& bullet : player->bullets) {
        if (bullet == nullptr) continue;
        if (bullet->Collide(std::move(rival.get()))) rival->Shrink();
      }
    }

    // Check on Rival collides Player
    if (rival->Collide(std::move(player.get()))) {
        if (rival->size > 1) {
          player->alive = false;
          return;
        }
        if (rival->size == 1 || player->size > 1) {
          player->Shrink();
          return;
        }
    }
}



int Game::GetScore() const { return score; }
int Game::GetSize() const { return player->size; }
