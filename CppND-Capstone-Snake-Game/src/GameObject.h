#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <iostream>
#include <typeinfo>
#include "SDL.h"
#include "map.h"

// forward declaration
class GameObject;
class Snake;
class Food;
class Bullet;
class RivalSnake;
class PlayerSnake;

// Base (abstract) class for game objects
class GameObject : public std::enable_shared_from_this<GameObject> {
  public:

    // Constructors
    GameObject() {}; // for constructing GameClasses
    GameObject(int grid_width, int grid_height,
               std::shared_ptr<Map<GameObject>> gameMap):
     grid_width(grid_width),
     grid_height(grid_height),
     origin_x(grid_width / 2),
     origin_y(grid_height / 2),
     _mtx() { setMap(std::move(gameMap)); }

    // Attributes
    float origin_x;
    float origin_y;
    std::vector<SDL_Point> body;

    // Virtual functions
    virtual void Update()=0;
    virtual void Initialize()=0;

    // Setters
    void setMap(std::shared_ptr<Map<GameObject>> gameMap) {
      map = std::move(gameMap);
    };
    void projectToMap() {
      putHeadtoMap();
      putBodytoMap();
    };
    void putHeadtoMap() {
      if(offGrid()) return;
      int head_x = static_cast<int>(origin_x);
      int head_y = static_cast<int>(origin_y);
      if(map->at(head_y, head_x) == nullptr) {
        (*map)[head_y][head_x] = shared_from_this();
      }
    };
    void putBodytoMap() {
      if(offGrid()) return;
      if (!body.empty()) {
        for (auto &part : body) {
          if (map->at(part.y, part.x) == nullptr) {
            (*map)[part.y][part.x] = shared_from_this();
          }
        }
      }
    };


    // template function to check if the object belongs to class `T`
    template <class T>
    bool isA() {
      return (dynamic_cast<T*>(this) != NULL);
    }

    // Check if object has fallen out of boundary
    bool offGrid() {
      if (origin_x <= 0 || origin_x >= grid_width
       || origin_y <= 0 || origin_y >= grid_height) {
         return true;
      }
      return false;
    }

  protected:
    mutable std::mutex _mtx;
    int grid_width;
    int grid_height;
    std::shared_ptr<Map<GameObject>> map;
    std::vector<std::thread> threads;
    bool initialized{false};

};


#endif
