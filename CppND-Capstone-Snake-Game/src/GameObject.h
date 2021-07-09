#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "SDL.h"

// forward declaration
class Snake;
class Food;

// Base class for game objects
class GameObject {
  public:
    GameObject() {};
    GameObject(int grid_width, int grid_height):
     grid_width(grid_width),
     grid_height(grid_height),
     origin_x(grid_width / 2),
     origin_y(grid_height / 2) {}
    float origin_x;
    float origin_y;
    std::vector<SDL_Point> body;
    friend Snake;
    friend Food;

  private:
    int grid_width;
    int grid_height;
};


#endif
