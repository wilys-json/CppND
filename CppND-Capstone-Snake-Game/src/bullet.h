#ifndef BULLET_H
#define BULLET_H

#include "Movables.h"

class Bullet : public Movables, public GameObject {
 public:
   ~Bullet();
   Bullet(int grid_width, int grid_height) :
     GameObject(grid_width, grid_height) {}
   void Upate() override;
   void Collide(const GameObject &other) override;

 private:
   Snake shooter*;
}

#endif
