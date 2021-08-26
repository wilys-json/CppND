#include "snake.h"

// const SwitchableColor::Color Snake::DefaultHeadColor = SwitchableColor::Color::kBlue;
// const SwitchableColor::Color Snake::DefaultBodyColor = SwitchableColor::Color::kWhite;

// Snake::~Snake() {
//   map = nullptr;
//   for (auto& bullet : bullets) bullet = nullptr;
//   for (auto& thread : threads) thread.join();
// }


void Snake::Initialize() {
  if (!initialized) {
    projectToMap();
    initialized = true;
  }
};


// void Snake::Update() {
//   // Capture previous head before updating
//   SDL_Point prev_cell{
//       static_cast<int>(origin_x),
//       static_cast<int>(
//           origin_y)};
//   UpdateHead();
//   UpdateBullets();
//   // Capture new head after updating
//   SDL_Point current_cell{
//       static_cast<int>(origin_x),
//       static_cast<int>(origin_y)};
//
//   // Update body if snake has moved to a new cell
//   if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
//     UpdateBody(current_cell, prev_cell);
//   }
// }


bool Snake::Collide(const GameObject* other) {
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

  // Check if snake's body is hit by other's head
  for (const auto &this_part : body) {
    if (this_part.x == static_cast<int>(other->origin_x)
    && this_part.y == static_cast<int>(other->origin_y)) return true;
  }

  return false;

}


void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      origin_y -= speed;
      break;

    case Direction::kDown:
      origin_y += speed;
      break;

    case Direction::kLeft:
      origin_x -= speed;
      break;

    case Direction::kRight:
      origin_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  origin_x = fmod(origin_x + grid_width, grid_width);
  origin_y = fmod(origin_y + grid_height, grid_height);
  putHeadtoMap();
}


// void Snake::UpdateBullets() {
//   if (!bullets.empty()) {
//     for (int i; i < bullets.size(); ++i) {
//       if (bullets[i] != nullptr) {
//         bullets[i]->Update();
//         if (bullets[i]->offGrid()) bullets[i] = nullptr;
//       }
//     }
//   }
// }


void Snake::UpdateBody(SDL_Point &current_head_cell,
                       SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died & put body to map
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
      return;
    }
    (*map)[item.y][item.x] = shared_from_this();
  }
}


void Snake::GrowBody() { growing = true; }


void Snake::setRandomInt(const int& randomNumber) {
  RandomInt = (randomNumber % 10);
}


void Snake::Consume(std::shared_ptr<Food>& food,
                    std::promise<Food::State> prmFoodState) {
  if (food->getState() != Food::State::kPoison) GrowBody();
  foodConsumed = food;
  // Ensure the snake enters one state at a time
  if (state == State::kNormal) {
    threads.emplace_back(std::thread(&Snake::Digest, this));
  }
  prmFoodState.set_value(std::move(foodConsumed->getState()));
}


// void Snake::Digest() {
//   std::lock_guard<std::mutex> Lock(_mtx);
//   switch(foodConsumed->getState()) {
//     case Food::State::kNormal:
//       speed += 0.02;
//       break;
//     case Food::State::kSuper:
//       std::cout << "enable shooter mode for ";
//       std::cout << ModeDuration+(size / 5) << " seconds." << std::endl;
//       state = State::kShooter;
//       speed += 0.04;
//       std::this_thread::sleep_for(std::chrono::seconds(ModeDuration+(size / 5)));
//       if (alive) {
//         state = State::kNormal;
//         speed -= 0.02;
//         std::cout << "shooter mode ends." << std::endl;
//       }
//       break;
//     case Food::State::kPoison:
//       std::cout << "Oh no! The snake will be poisoned for ";
//       std::cout << ModeDuration+(size / 5) << " seconds." << std::endl;
//       state = State::kPoisoned;
//       speed /= 2;
//       std::this_thread::sleep_for(std::chrono::seconds(ModeDuration+(size / 5)));
//       if (alive) {
//         state = State::kNormal;
//         speed *= 2;
//         std::cout << "poisoned mode ends." << std::endl;
//       }
//       break;
//     case Food::State::kSpeedup:
//       std::cout << "Wow! The snake will rush for ";
//       std::cout << ModeDuration+(size / 5) << " seconds." << std::endl;
//       state = State::kSpeeding;
//       speed *= 2;
//       std::this_thread::sleep_for(std::chrono::seconds(ModeDuration+(size / 5)));
//       if (alive) {
//         state = State::kNormal;
//         speed /= 2;
//         std::cout << "speeding mode ends." << std::endl;
//       }
//       break;
//   }
// }
//
//
// void Snake::Shoot() {
//   if(!alive || state != State::kShooter) return;
//   bullets.push_back(
//     std::move(std::make_shared<Bullet>(grid_width,grid_height, map,
//         std::dynamic_pointer_cast<Snake>(shared_from_this()))));
// }

// Deprecated : Implementation of Rule of Five

// Snake::Snake(const Snake& source) {
//     std::shared_lock<std::shared_mutex> _sourceLock(source._mtx, std::defer_lock);
//     grid_width = source.grid_width;
//     grid_height = source.grid_height;
//     direction = source.direction;
//     speed = source.speed;
//     body = source.body;
//     size = source.size;
//     alive = source.alive;
//     map = source.map;
//     initialized = source.initialized;
//     growing = source.growing;
//     state = source.state;
//     origin_x = source.origin_x;
//     origin_y = source.origin_y;
//     if (!source.bullets.empty()) {
//       for (auto& bullet : source.bullets) {
//         if (bullet != nullptr) bullets.push_back(bullet);
//      }
//    }
//     // if (!source.threads.empty()) {
//     //   for (auto& thread : source.threads) {
//     //     std::thread sourceThread = thread;
//     //     threads.push_back(std::move(sourceThread));
//     //   }
//     // }
// }
//
//
// Snake& Snake::operator=(const Snake& source) {
//   if (this!=&source) {
//     std::unique_lock<std::shared_mutex> _thisLock(_mtx, std::defer_lock);
//     std::shared_lock<std::shared_mutex> _sourceLock(source._mtx, std::defer_lock);
//     std::lock(_thisLock, _sourceLock);
//     grid_width = source.grid_width;
//     grid_height = source.grid_height;
//     direction = source.direction;
//     speed = source.speed;
//     body = source.body;
//     size = source.size;
//     alive = source.alive;
//     map = source.map;
//     initialized = source.initialized;
//     growing = source.growing;
//     state = source.state;
//     origin_x = source.origin_x;
//     origin_y = source.origin_y;
//     if (!source.bullets.empty()) {
//       for (auto& bullet : source.bullets) {
//         if (bullet != nullptr) bullets.push_back(bullet);
//       }
//     }
//     // if (!source.threads.empty()) {
//     //   for (auto& thread : source.threads) {
//     //     std::thread sourceThread = thread;
//     //     threads.push_back(std::move(sourceThread));
//     //   }
//     // }
//   }
//   return *this;
// }
//
//
// Snake::Snake(Snake&& source) {
//   std::shared_lock<std::shared_mutex> _sourceLock(source._mtx, std::defer_lock);
//   threads = std::move(source.threads);
//   grid_width = source.grid_width;
//   grid_height = source.grid_height;
//   direction = source.direction;
//   speed = source.speed;
//   body = source.body;
//   size = source.size;
//   alive = source.alive;
//   map = source.map;
//   initialized = source.initialized;
//   growing = source.growing;
//   state = source.state;
//   origin_x = source.origin_x;
//   origin_y = source.origin_y;
//   if (!source.bullets.empty()) {
//     for (auto& bullet : source.bullets) {
//       if (bullet != nullptr) bullets.push_back(std::move(bullet));
//     }
//   }
//   if (!source.threads.empty()) {
//     for (auto& thread : source.threads) {
//       threads.push_back(std::move(thread));
//     }
//   }
//
//   source.grid_width = 0;
//   source.grid_height = 0;
//   source.speed = 0.0;
//   source.body.clear();
//   source.size = 0;
//   source.alive = NULL;
//   source.map = nullptr;
//   source.initialized = NULL;
//   source.growing = NULL;
//   source.origin_x = 0.0;
//   source.origin_y = 0.0;
//   source.bullets.clear();
//
// }
//
// Snake& Snake::operator=(Snake&& source) {
//   if (this!=&source) {
//     std::unique_lock<std::shared_mutex> _thisLock(_mtx, std::defer_lock);
//     std::shared_lock<std::shared_mutex> _sourceLock(source._mtx, std::defer_lock);
//     std::lock(_thisLock, _sourceLock);
//     threads = std::move(source.threads);
//     grid_width = source.grid_width;
//     grid_height = source.grid_height;
//     direction = source.direction;
//     speed = source.speed;
//     body = source.body;
//     size = source.size;
//     alive = source.alive;
//     map = source.map;
//     initialized = source.initialized;
//     growing = source.growing;
//     state = source.state;
//     origin_x = source.origin_x;
//     origin_y = source.origin_y;
//     if (!source.bullets.empty()) {
//       for (auto& bullet : source.bullets) {
//         if (bullet != nullptr) bullets.push_back(bullet);
//       }
//     }
//     if (!source.threads.empty()) {
//       for (auto& thread : source.threads) {
//         threads.push_back(std::move(thread));
//       }
//     }
//
//     source.grid_width = 0;
//     source.grid_height = 0;
//     source.speed = 0.0;
//     source.body.clear();
//     source.size = 0;
//     source.alive = NULL;
//     source.map = nullptr;
//     source.initialized = NULL;
//     source.growing = NULL;
//     source.origin_x = 0.0;
//     source.origin_y = 0.0;
//     source.bullets.clear();
//   }
//
//   return *this;
//
// }
