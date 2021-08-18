#include "snake.h"
#include "bullet.h"
#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>


Snake::~Snake() {
  map = nullptr;
  for (auto & bullet : bullets) bullet = nullptr;
}


Snake::Snake(const Snake& source) {
  grid_width = source.grid_width;
  grid_height = source.grid_height;
  direction = source.direction;
  speed = source.speed;
  body = source.body;
  size = source.size;
  alive = source.alive;
  map = source.map;
  initialized = source.initialized;
  growing = source.growing;
  shooterMode = source.shooterMode;
  origin_x = source.origin_x;
  origin_y = source.origin_y;
  if (!source.bullets.empty()) {
    for (auto& bullet : source.bullets) {
      if (bullet != nullptr) bullets.push_back(bullet);
    }
  }
}


Snake& Snake::operator=(const Snake& source) {
  grid_width = source.grid_width;
  grid_height = source.grid_height;
  direction = source.direction;
  speed = source.speed;
  body = source.body;
  size = source.size;
  alive = source.alive;
  map = source.map;
  initialized = source.initialized;
  growing = source.growing;
  shooterMode = source.shooterMode;
  origin_x = source.origin_x;
  origin_y = source.origin_y;
  if (!source.bullets.empty()) {
    for (auto& bullet : source.bullets) {
      if (bullet != nullptr) bullets.push_back(bullet);
    }
  }
  return *this;
}


Snake::Snake(Snake&& source) {
  grid_width = source.grid_width;
  grid_height = source.grid_height;
  direction = source.direction;
  speed = source.speed;
  body = source.body;
  size = source.size;
  alive = source.alive;
  map = source.map;
  initialized = source.initialized;
  growing = source.growing;
  shooterMode = source.shooterMode;
  origin_x = source.origin_x;
  origin_y = source.origin_y;
  if (!source.bullets.empty()) {
    for (auto& bullet : source.bullets) {
      if (bullet != nullptr) bullets.push_back(bullet);
    }
  }

  source.grid_width = 0;
  source.grid_height = 0;
  source.speed = 0.0;
  source.body.clear();
  source.size = 0;
  source.alive = NULL;
  source.map = nullptr;
  source.initialized = NULL;
  source.growing = NULL;
  source.shooterMode = NULL;
  source.origin_x = 0.0;
  source.origin_y = 0.0;
  source.bullets.clear();

}

Snake& Snake::operator=(Snake&& source) {

  grid_width = source.grid_width;
  grid_height = source.grid_height;
  direction = source.direction;
  speed = source.speed;
  body = source.body;
  size = source.size;
  alive = source.alive;
  map = source.map;
  initialized = source.initialized;
  growing = source.growing;
  shooterMode = source.shooterMode;
  origin_x = source.origin_x;
  origin_y = source.origin_y;
  if (!source.bullets.empty()) {
    for (auto& bullet : source.bullets) {
      if (bullet != nullptr) bullets.push_back(bullet);
    }
  }

  source.grid_width = 0;
  source.grid_height = 0;
  source.speed = 0.0;
  source.body.clear();
  source.size = 0;
  source.alive = NULL;
  source.map = nullptr;
  source.initialized = NULL;
  source.growing = NULL;
  source.shooterMode = NULL;
  source.origin_x = 0.0;
  source.origin_y = 0.0;
  source.bullets.clear();

  return *this;

}

void Snake::enableShooterMode() {
  // std::cout << "enable shooter mode." << std::endl;
  shooterMode = true;
  // std::this_thread::sleep_for(std::chrono::seconds(5));
  // std::cout << "shooter mode ends." << std::endl;
  // shooterMode = false;
}

void Snake::Initialize() {
  if (!initialized) {
    putHeadtoMap();
    putBodytoMap();
    initialized = true;
  }
};

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(origin_x),
      static_cast<int>(
          origin_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  UpdateBullets();
  SDL_Point current_cell{
      static_cast<int>(origin_x),
      static_cast<int>(origin_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

bool Snake::Collide(const GameObject &other) {

  int this_x = static_cast<int>(origin_x);
  int this_y = static_cast<int>(origin_y);

  // Check if head touches head
  if (static_cast<int>(other.origin_x) == this_x
    && static_cast<int>(other.origin_y) == this_y)
    return true;

  // Check if snake's head hit other's body
  for (const auto &other_part : other.body) {
    if (other_part.x == this_x && other_part.y == this_y) return true;
  }

  // Check if snake's body is hit by other's head
  for (const auto &this_part : body) {
    if (this_part.x == static_cast<int>(other.origin_x)
    && this_part.y == static_cast<int>(other.origin_y)) return true;
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

void Snake::UpdateBullets() {
  for (auto &bullet : bullets) bullet->Update();
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }


  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
      return;
    }
    (*map)[item.y][item.x] = shared_from_this();
  }
}

void Snake::GrowBody() { growing = true; }

void Snake::Shoot() {
  if(!alive || !shooterMode) return;
  std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(grid_width, grid_height, map, this);
  // Bullet* bullet = new Bullet(grid_width, grid_height, this);
  bullets.push_back(std::move(bullet));
}

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(origin_x) && y == static_cast<int>(origin_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}
