#include "snake.h"
#include <cmath>
#include <iostream>

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
}

void Snake::UpdateBullets() {
  for (auto &bullet : bullets) bullet.Update();
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
    }
  }
}

void Snake::GrowBody() { growing = true; }

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
