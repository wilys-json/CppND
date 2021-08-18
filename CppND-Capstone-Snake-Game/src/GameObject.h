#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <memory>
#include <mutex>
#include <iostream>
#include <typeinfo>
#include "SDL.h"
#include "map.h"

// forward declaration
class Snake;
class Food;
class Bullet;
class Map;

// Base class for game objects
class GameObject : public std::enable_shared_from_this<GameObject> {
  public:

    // constructors
    GameObject() {};
    GameObject(int grid_width, int grid_height, std::shared_ptr<Map> gameMap):
     grid_width(grid_width),
     grid_height(grid_height),
     origin_x(grid_width / 2),
     origin_y(grid_height / 2) { setMap(gameMap); }

    // attributes
    float origin_x;
    float origin_y;
    std::vector<SDL_Point> body;

    // virtual functions
    virtual void Update()=0;
    virtual void Initialize()=0;

    // friend class, also derived classes
    friend Snake;
    friend Food;
    friend Bullet;

    // common methods
    void setMap(std::shared_ptr<Map> gameMap) { map = std::move(gameMap);};

    void putHeadtoMap() {
      int head_x = static_cast<int>(origin_x);
      int head_y = static_cast<int>(origin_y);
      (*map)[head_y][head_x] = shared_from_this();
    };
    void putBodytoMap() {
      if (!body.empty()) {
        for (auto &part : body) (*map)[part.y][part.x] = shared_from_this();
      }
    };

    bool operator==(const GameObject& other) {
      return typeid(*this) == typeid(other);
    }

  protected:
    static std::mutex _mtx;

  private:
    int grid_width;
    int grid_height;
    std::shared_ptr<Map> map;
    bool initialized{false};
};


#endif
