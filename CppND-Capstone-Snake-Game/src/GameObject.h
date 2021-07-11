#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <memory>
#include <mutex>
#include "SDL.h"

// forward declaration
class Snake;
class Food;
class Bullet;

// Base class for game objects
class GameObject : public std::enable_shared_from_this<GameObject> {
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
    void setMap(std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameObject>>>> &map)
    {
      map = map;
      putHeadtoMap();
    }
    void putHeadtoMap() {
      int head_x = static_cast<int>(origin_x);
      int head_y = static_cast<int>(origin_y);
      (*map)[head_y][head_x] = shared_from_this();
    }
    void putBodytoMap() {
      if (!body.empty()) {
        for (auto &part : body) (*map)[part.y][part.x] = shared_from_this();
    }
  }
    friend Snake;
    friend Food;
    friend Bullet;
  protected:
    static std::mutex _mtx;
  private:
    int grid_width;
    int grid_height;
    std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameObject>>>> map;
};


#endif
