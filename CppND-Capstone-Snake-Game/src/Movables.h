#ifndef MOVABLES_H
#define MOVABLES_H

#include "SDL.h"
#include "GameObject.h"

// interface for movable objects
class Movables
{
 public:
   enum class Direction { kUp, kDown, kLeft, kRight };
   Movables(Direction direction, float speed) :
    direction(direction),
    speed(speed) {};
   Movables() {};
   virtual ~Movables() {};
   Direction direction;
   float speed;
   virtual void Update() = 0;
   virtual bool Collide(const GameObject &other) = 0;
};

#endif
