#include "player.h"
#include "bullet.h"
#include "food.h"


PlayerSnake::~PlayerSnake() {
  map = nullptr;
  if (!bullets.empty()) for (auto& bullet : bullets) bullet = nullptr;
}


void PlayerSnake::UpdateBullets() {
  if (!bullets.empty()) {
    for (int i; i < bullets.size(); ++i) {
      if (bullets[i] != nullptr) {
        bullets[i]->Update();
        if (bullets[i]->offGrid()) bullets[i] = nullptr;
      }
    }
  }
}


const Color PlayerSnake::getDefaultHeadColor() {
  return DefaultColors::PlayerHeadColor;
}


const Color PlayerSnake::getDefaultBodyColor() {
  return DefaultColors::PlayerBodyColor;
}


void PlayerSnake::Update() {
  // Capture previous head before updating
  SDL_Point prev_cell{
      static_cast<int>(origin_x),
      static_cast<int>(
          origin_y)};
  UpdateHead();
  UpdateBullets();
  // Capture new head after updating
  SDL_Point current_cell{
      static_cast<int>(origin_x),
      static_cast<int>(origin_y)};

  // Update body if snake has moved to a new cell
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }

}


bool PlayerSnake::Collide(const GameObject* other) {
  if (other == this) return false;

  int this_x = static_cast<int>(origin_x);
  int this_y = static_cast<int>(origin_y);

  // Check if head touches head
  if (static_cast<int>(other->origin_x) == this_x
    && static_cast<int>(other->origin_y) == this_y)
    return true;

  // Check if snake's head hit other's body
  for (const auto &other_part : other->body) {
    if (other_part.x == this_x && other_part.y == this_y) return true;
  }

  return false;

}


void PlayerSnake::Shrink() {
  if (size > 1 && !body.empty()) {
    body.erase(body.begin());
    size--;
    } else { alive = false; }
}


void PlayerSnake::Digest() {

  std::lock_guard<std::mutex> Lock(_mutex);
  switch(foodConsumed->getState()) {
    case Food::State::kNormal:
      speed += 0.02;
      break;
    case Food::State::kSuper:
      std::cout << "enable shooter mode for ";
      std::cout << RandomInt+(size / 5) << " seconds." << std::endl;
      state = State::kShooter;
      speed += 0.04;
      std::this_thread::sleep_for(std::chrono::seconds(RandomInt+(size / 5)));
      if (alive) {
        state = State::kNormal;
        speed -= 0.02;
        std::cout << "shooter mode ends." << std::endl;
      }
      break;
    case Food::State::kPoison:
      std::cout << "Oh no! The snake will be poisoned for ";
      std::cout << RandomInt+(size / 5) << " seconds." << std::endl;
      state = State::kPoisoned;
      speed /= 2;
      std::this_thread::sleep_for(std::chrono::seconds(RandomInt+(size / 5)));
      if (alive) {
        state = State::kNormal;
        speed *= 2;
        std::cout << "poisoned mode ends." << std::endl;
      }
      break;
    case Food::State::kSpeedup:
      std::cout << "Wow! The snake will rush for ";
      std::cout << RandomInt+(size / 5) << " seconds." << std::endl;
      state = State::kSpeeding;
      speed *= 2;
      std::this_thread::sleep_for(std::chrono::seconds(RandomInt+(size / 5)));
      if (alive) {
        state = State::kNormal;
        speed /= 2;
        std::cout << "speeding mode ends." << std::endl;
      }
      break;
  }
}


void PlayerSnake::Shoot() {
  if(!alive || state != State::kShooter) return;
  bullets.push_back(
    std::move(std::make_shared<Bullet>(grid_width,grid_height, map,
        std::dynamic_pointer_cast<PlayerSnake>(shared_from_this()))));
}
