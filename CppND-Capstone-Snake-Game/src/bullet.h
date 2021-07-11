#ifndef BULLET_H
#define BULLET_H

#include <iostream>
#include "Movables.h"
#include "SwitchableColor.h"
#include "snake.h"


class Bullet : public Movables, public SwitchableColor, public GameObject {
 public:
   Bullet() {};
   ~Bullet(){};
   Bullet(int grid_width, int grid_height, Snake* snake) :
     shooter(snake),
     GameObject(grid_width, grid_height),
     Movables(snake->direction, snake->speed + 0.2f) {
       origin_x = snake->origin_x;
       origin_y = snake->origin_y;
       color = Color::kRed;
     };
   void Update() override;
   bool Collide(const GameObject &other) override;

 private:
   Snake* shooter;
   void UpdateHead();
   void Blink() override;
   void removeThisFromShooter();
   void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
};

#endif
