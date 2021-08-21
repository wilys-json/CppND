#ifndef BULLET_H
#define BULLET_H

#include <iostream>
#include <memory>
#include "Movables.h"
#include "SwitchableColor.h"
#include "snake.h"

class Map;

class Bullet : public Movables, public SwitchableColor, public GameObject {
 public:
   Bullet() {};
   ~Bullet();
   Bullet(int grid_width,
          int grid_height,
          std::shared_ptr<Map> gameMap,
          std::shared_ptr<Snake> snake) :
     shooter(snake),
     GameObject(grid_width, grid_height, gameMap),
     Movables(snake->direction, snake->speed + 0.2f),
     SwitchableColor() { Initialize(); };

   // Implement GameObject
   void Initialize() override;
   void Update() override;
   bool Collide(const GameObject &other) override;

 private:
   std::shared_ptr<Snake> shooter;
   void UpdateHead();
   void Blink() override;
   void removeThisFromShooter();
   // void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
};

#endif
