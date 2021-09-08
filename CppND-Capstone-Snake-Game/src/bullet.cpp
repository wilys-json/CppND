#include "bullet.h"


Bullet::~Bullet() {
  map = nullptr;
  shooter = nullptr;
}


void Bullet::Initialize() {
  if(!initialized) {
    origin_x = shooter->origin_x;
    origin_y = shooter->origin_y;
    color = Color::kRed;
    initialized = true;
  }
}


void Bullet::Update() {

  SDL_Point prev_cell{
      static_cast<int>(origin_x),
      static_cast<int>(
          origin_y)};
  UpdateHead();
  Blink();
  putHeadtoMap();

  SDL_Point current_cell{
      static_cast<int>(origin_x),
      static_cast<int>(origin_y)};
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

  if (offGrid()) {
    origin_x = -1;
    origin_y = -1;
  }
}


void Bullet::Blink() {
  color = (color == Color::kRed) ? Color::kYellow : Color::kRed;
}


bool Bullet::Collide(const GameObject* other) {

  if (this == other) return false;

  int this_x = static_cast<int>(origin_x);
  int this_y = static_cast<int>(origin_y);

  // Check if head touches head
  if (static_cast<int>(other->origin_x) == this_x
    && static_cast<int>(other->origin_y) == this_y)
    return true;

  // Check if bullet's head hit other's body
  for (const auto &other_part : other->body) {
    if (other_part.x == this_x && other_part.y == this_y) return true;
  }

  return false;

}
