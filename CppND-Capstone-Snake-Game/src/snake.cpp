#include "snake.h"


void Snake::Initialize() {
  if (!initialized) {
    projectToMap();
    initialized = true;
  }
};


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


void Snake::Consume(std::shared_ptr<Food> food,
                    std::promise<std::shared_ptr<Snake>> prmFoodConsumption) {
  if (food->getState() != Food::State::kPoison) GrowBody();
  foodConsumed = food;
  // Ensure the snake enters one state at a time
  if (state == State::kNormal) {
    threads.emplace_back(std::thread(&Snake::Digest, this));
  }
  prmFoodConsumption.set_value(std::dynamic_pointer_cast<Snake>(shared_from_this()));
}



// Implementation of Rule of Five

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
