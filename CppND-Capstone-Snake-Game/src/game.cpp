
#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include "SDL.h"
#include "game.h"
#include "map.h"
#include "food.h"
#include "snake.h"
#include "controller.h"
#include "renderer.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) { Initialize(grid_width, grid_height); }

void Game::Initialize(std::size_t grid_width, std::size_t grid_height) {
  map = std::make_shared<Map>(grid_height, grid_width);
  snake = std::make_shared<Snake>(grid_width, grid_height, map);
  snake->Initialize();
  objectPool.push_back(snake);
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
    controller.HandleInput(running, snake);
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
    if ((*map)[y][x] != nullptr) continue;

    // Reassign the Food pointer pointing to new Food
    for (auto &gameobject : objectPool) {
      if (*gameobject == gameClasses.FOOD) {
        gameobject.reset(new Food(x, y, map));
        std::shared_ptr<Food> food = std::dynamic_pointer_cast<Food>(gameobject);
        food->Initialize();
        if (score > 0 && score % 5 == 0) food->setState(Food::State::kSuper);
        if (x % 5 == 0 && y % 5 != 0) {
          food->setState(Food::State::kPoison);
        }
        if (x % 5 == 0 && y % 5 == 0) {
          food->setState(Food::State::kSpeedup);
        }
        return;
     }
   }

   // Initialize Food
   std::shared_ptr<Food> newFood = std::make_shared<Food>(x, y, map);
   newFood->Initialize();
   objectPool.push_back(newFood);
   return;
 }
}

void Game::Update() {

  std::shared_ptr<Food> food;
  std::shared_ptr<Snake> snake;
  int randomInt = random_w(engine);

  clearMap();

  for (auto& gameobject : objectPool) {
    if(*gameobject == gameClasses.FOOD) food = std::dynamic_pointer_cast<Food>(gameobject);
    if(*gameobject == gameClasses.SNAKE) {
      snake = std::dynamic_pointer_cast<Snake>(gameobject);
      if (!snake->alive) return;
    }
    gameobject->Update();
  }

  // Check if there's food over here
  if (snake->Collide(*food)) {
    if (food->getState() != Food::State::kPoison) score++;
    snake->setModeDuration(randomInt);
    snake->Consume(food);
    PlaceFood();
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake->size; }
