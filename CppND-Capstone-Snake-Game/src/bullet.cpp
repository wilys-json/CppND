#include <vector>
#include "bullet.h"

void Bullet::removeThisFromShooter() {
  for (auto &bullet : shooter->bullets) {
    if (bullet.get() == this) bullet.reset();
    // if (bullet == this) delete bullet;
    return;
  }
  shooter = nullptr;
}

void Bullet::Update() {
  SDL_Point prev_cell{
      static_cast<int>(origin_x),
      static_cast<int>(
          origin_y)};
  UpdateHead();
  Blink();
  SDL_Point current_cell{
      static_cast<int>(origin_x),
      static_cast<int>(origin_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Bullet::UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell) {
    body.push_back(prev_cell);
    body.erase(body.begin());
}

void Bullet::UpdateHead() {

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

  if (origin_x == 0 || origin_x >= grid_width
   || origin_y == 0 || origin_y >= grid_height) {
     origin_x = -1;
     origin_y = -1;
     removeThisFromShooter();
   }

}

void Bullet::Blink() {
  color = (color == Color::kRed) ? Color::kYellow : Color::kRed;
}

bool Bullet::Collide(const GameObject &other) {

  int this_x = static_cast<int>(origin_x);
  int this_y = static_cast<int>(origin_y);

  // Check if head touches head
  if (static_cast<int>(other.origin_x) == this_x
    && static_cast<int>(other.origin_y) == this_y)
    return true;

  // Check if bullet's head hit other's body
  for (const auto &other_part : other.body) {
    if (other_part.x == this_x && other_part.y == this_y) return true;
  }

  return false;

}
