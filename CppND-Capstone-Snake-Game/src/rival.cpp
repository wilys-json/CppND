#include <thread>
#include <chrono>
#include "rival.h"
#include "player.h"
#include "food.h"


RivalSnake::~RivalSnake() {
  map = nullptr;
  alive = false;
  walkThread.detach();
  for(auto& thread : threads) thread.detach();
}

void RivalSnake::Initialize() {
  origin_x /= 2;
  origin_y /= 2;
  if (!initialized) {
    projectToMap();
    initialized = true;
  }
  walkThread = std::thread(&RivalSnake::RandomWalk, this);
};

void RivalSnake::Initialize(const int& score, const int& x, const int& y) {
    origin_x = x;
    origin_y = y;
    speed += (static_cast<float>(score) * 0.01);
    EnhanceSense((2+(score/10))*2);
    if (!initialized) {
      projectToMap();
      initialized = true;
    }
    walkThread = std::thread(&RivalSnake::RandomWalk, this);
};


void RivalSnake::Sense() {
  int sensorMapSize = (SensingRange * 2)+1;
  for(int i = 0; i < sensorMapSize; ++i) {
    for(int j = 0; j < sensorMapSize; ++j) {
      // if(i - SensingRange == 0 && j - SensingRange == 0) {
      //   std::cout << " " << std::endl;
      //   continue;
      // }
      int sensor_x = static_cast<int>(origin_x) + j - SensingRange;
      int sensor_y = static_cast<int>(origin_y) + i - SensingRange;
      if (offGrid(sensor_x, sensor_y)|| map->at(sensor_y, sensor_x)->isA<RivalSnake>())
        continue;

      // Aim food first
      if(map->at(sensor_y, sensor_x)->isA<Food>()) {
        mode = Mode::kBattle;
        Aim(sensor_x, sensor_y);
        continue;
      }
      // Then aim player
      if(map->at(sensor_y, sensor_x)->isA<PlayerSnake>()) {
        mode = ((std::dynamic_pointer_cast<Snake>(map->at(sensor_y, sensor_x))->getState() == Snake::State::kPoisoned) ? Mode::kBattle : Mode::kEscape);
        // mode = Mode::kBattle;  // Always attack player [for testing]
        // mode = Mode::kEscape; // Always escape from player [for testing]
        Aim(sensor_x, sensor_y);
        continue;
      }
    }
  }
}


void RivalSnake::Aim(const int& sensor_x, const int& sensor_y) {

  int current_x = static_cast<int>(origin_x);
  int current_y = static_cast<int>(origin_y);

   if (sensor_y > current_y)
      direction = ((mode == Mode::kEscape) ? Direction::kUp :
                  ((direction == Direction::kUp && size > 1) ?
                   Direction::kLeft : Direction::kDown));
   else if (sensor_y < current_y)
      direction = ((mode == Mode::kEscape) ? Direction::kDown :
                  ((direction == Direction::kDown && size > 1) ?
                   Direction::kRight : Direction::kUp));
   else if (sensor_y == current_y) {
   if (sensor_x < current_x)
      direction = ((mode == Mode::kEscape) ? Direction::kRight :
                  ((direction == Direction::kRight && size > 1) ?
                   Direction::kUp : Direction::kLeft));
   else if (sensor_x > current_x)
      direction = ((mode == Mode::kEscape) ? Direction::kLeft :
                  ((direction == Direction::kLeft && size > 1) ?
                   Direction::kDown : Direction::kRight));
   }
   walkConditionVariable.notify_one();
}


void RivalSnake::Shrink() {
  if (size > 1 && !body.empty()) {
    body.erase(body.begin());
    size--;
    SensingRange -= 2;
    } else { alive = false; }
}

void RivalSnake::EnhanceSense(int i=2) {
    SensingRange = (((SensingRange + i) * 2 + 1 >= grid_width) ?
                    grid_width : SensingRange + i);
}

void RivalSnake::RandomWalk() {
  std::lock_guard<std::mutex> Lock(_walkMutex);

  while(alive) {
  int choice = RandomInt % 4;
  switch (choice) {
    case 0:
      direction = ((direction == Direction::kDown) ?
                    Direction::kDown : Direction::kUp);
      break;
    case 1:
      direction = ((direction == Direction::kUp) ?
                    Direction::kUp : Direction::kDown);
      break;
    case 2:
      direction = ((direction == Direction::kLeft) ?
                    Direction::kLeft : Direction::kRight);
      break;
    case 3:
      direction = ((direction == Direction::kRight) ?
                    Direction::kRight : Direction::kLeft);
      break;
    }
    walkConditionVariable.notify_one();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void RivalSnake::Update() {

  // Capture previous head before updating
  SDL_Point prev_cell{
      static_cast<int>(origin_x),
      static_cast<int>(
          origin_y)};

  // sensorMap->print(); // print to debug
  UpdateHead();
  // Capture new head after updating
  SDL_Point current_cell{
      static_cast<int>(origin_x),
      static_cast<int>(origin_y)};

  // Update body if snake has moved to a new cell
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
        }

  Sense();

}

const Color RivalSnake::getDefaultHeadColor() {
  return DefaultColors::RivalHeadColor;
}


const Color RivalSnake::getDefaultBodyColor() {
  return DefaultColors::RivalBodyColor;
}


void RivalSnake::Digest() {
  std::lock_guard<std::mutex> Lock(_mutex);
  switch(foodConsumed->getState()) {
    case Food::State::kNormal:
      speed += 0.02;
      EnhanceSense();
      break;
    case Food::State::kSuper:
      break;
    case Food::State::kPoison:
      state = State::kPoisoned;
      speed /= 2;
      EnhanceSense(-2);
      std::this_thread::sleep_for(std::chrono::seconds(RandomInt+(size / 5)));
      if (alive) {
        state = State::kNormal;
        speed *= 2;
        EnhanceSense();
      }
      break;
    case Food::State::kSpeedup:
      state = State::kSpeeding;
      speed *= 2;
      EnhanceSense(4);
      std::this_thread::sleep_for(std::chrono::seconds(RandomInt+(size / 5)));
      if (alive) {
        state = State::kNormal;
        speed /= 2;
        EnhanceSense(-2);
      }
      break;
  }
}
