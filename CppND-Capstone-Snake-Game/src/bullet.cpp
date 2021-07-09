#include <vector>
#include "bullet.h"

Bullet::~Bullet() {
  for (auto &bullet : shooter->bullets) {
    if (bullet == this) delete bullet;
    return;
  }
}

void Bullet::Update() {

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
     ~Bullet();
   }

}

void Bullet::Collide(const GameObject &other) {

}
