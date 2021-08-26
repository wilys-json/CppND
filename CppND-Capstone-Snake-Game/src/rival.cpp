#include "rival.h"
#include "food.h"


RivalSnake::~RivalSnake() {
  map = nullptr;
  for (auto& thread : threads) thread.join();
}

void RivalSnake::Move() {

}

void RivalSnake::Update() {
  // Capture previous head before updating
  SDL_Point prev_cell{
      static_cast<int>(origin_x),
      static_cast<int>(
          origin_y)};
  Move();
  UpdateHead();
  // Capture new head after updating
  SDL_Point current_cell{
      static_cast<int>(origin_x),
      static_cast<int>(origin_y)};

  // Update body if snake has moved to a new cell
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

const Color RivalSnake::getDefaultHeadColor() {
  return DefaultColors::RivalHeadColor;
}
const Color RivalSnake::getDefaultBodyColor() {
  return DefaultColors::RivalBodyColor;
}


void RivalSnake::Digest() {
  std::lock_guard<std::mutex> Lock(_mtx);
  switch(foodConsumed->getState()) {
    case Food::State::kNormal:
      speed += 0.02;
      SensingRange++;
      break;
    case Food::State::kSuper:
      break;
    case Food::State::kPoison:
      std::cout << "Ah ha! The rival snake will be poisoned for ";
      std::cout << RandomInt+(size / 5) << " seconds." << std::endl;
      state = State::kPoisoned;
      speed /= 2;
      SensingRange -= 2;
      std::this_thread::sleep_for(std::chrono::seconds(RandomInt+(size / 5)));
      if (alive) {
        state = State::kNormal;
        speed *= 2;
        SensingRange += 2;
        std::cout << "Rival snake resumes normal." << std::endl;
      }
      break;
    case Food::State::kSpeedup:
      std::cout << "Oops! The rival snake will rush for ";
      std::cout << RandomInt+(size / 5) << " seconds." << std::endl;
      state = State::kSpeeding;
      speed *= 2;
      SensingRange += 2;
      std::this_thread::sleep_for(std::chrono::seconds(RandomInt+(size / 5)));
      if (alive) {
        state = State::kNormal;
        speed /= 2;
        SensingRange -=2;
        std::cout << "Rival snake resumes normal." << std::endl;
      }
      break;
  }
}
