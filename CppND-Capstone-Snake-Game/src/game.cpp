#include <future>
#include "game.h"
#include "snake.h"


template <typename T>
T MessageQueue<T>::receive()
{

  std::unique_lock<std::mutex> Lock(_mutex);
  _condition.wait(Lock, [this]{return !_queue.empty();});
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
      random_h(0, static_cast<int>(grid_height - 1)) { Initialize(grid_width, grid_height); }

void Game::Initialize(std::size_t grid_width, std::size_t grid_height) {
  map = std::make_shared<Map<GameObject>>(grid_height, grid_width);
  player = std::make_shared<PlayerSnake>(grid_width, grid_height, map);
  player->Initialize();
  objectPool.push_back(player);
  // objectPool.push_back(std::make_shared<Food>()); // push back dummy food first
  PlaceFood();
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

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);

    // Check that the location is not occupied by a snake item before placing
    // food.
    if (map->at(y, x) != nullptr) continue;

    // Reassign the Food pointer pointing to new Food
    for (auto &gameobject : objectPool) {
      if (gameobject->isA<Food>()) {
        gameobject.reset(new Food(x, y, map));
        food = std::dynamic_pointer_cast<Food>(gameobject);
        food->Initialize(score, x, y);
        return;
     }
   }

   // Initialize Food
   food = std::make_shared<Food>(x, y, map);
   food->Initialize();
   objectPool.push_back(food);
   return;
 }
}

void Game::Update() {

  if(!player->alive) return;

  // std::shared_ptr<Snake> eater;
  int randomInt = random_w(engine);

  clearMap();
  for (auto& gameobject : objectPool) {
    gameobject->Update();
    if(gameobject->isA<Snake>()) {
      std::shared_ptr<Snake> snake = std::dynamic_pointer_cast<Snake>(gameobject);
      snake->setRandomInt(randomInt);
      if (snake->Collide(std::move(food.get())))
      {
        std::promise<Food::State> prmFoodState;
        // std::future<Food::State> ftrFoodState = prmFoodState.get_future();
        // foodConsumptionPromises.push_back(prmFoodState)
        foodConsumptionThreads.push_back(std::thread(&Snake::Consume,
          snake, food, std::move(prmFoodState)));
      }
    }
  }

  UpdateScore();



}


void Game::UpdateScore() {
  if (!foodConsumptionFutures.empty()) {
    for (auto& foodConsumptionFuture : foodConsumptionFutures) {
      foodConsumptionFuture.wait();
      std::cout << "future wait finish" << std::endl;
      foodConsumptionQueue.send(std::move(foodConsumptionFuture.get()));
      std::cout << "future sent to msg queue" << std::endl;
    }

    for (auto& thread : foodConsumptionThreads) thread.join();


    if (foodConsumptionQueue.receive() != Food::State::kPoison) {
         score++;
         PlaceFood();
    }
  }
}


int Game::GetScore() const { return score; }
int Game::GetSize() const { return player->size; }
