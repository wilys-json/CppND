#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <memory>
#include <thread>
#include "Movables.h"
#include "SDL.h"

class Bullet;
class Map;

class Snake : public Movables, public GameObject {
 public:
  // constructor
  Snake() {};
  Snake(int grid_width, int grid_height, std::shared_ptr<Map> gameMap)
      : GameObject(grid_width, grid_height, gameMap),
        Movables(Direction::kUp, 0.1f) {}

  // destructor
  ~Snake();

  // Move & Copy Semantics
  Snake(const Snake& source); // copy constructor
  Snake& operator=(const Snake& source); // copy assignment operator
  Snake(Snake&& source); // move constructor
  Snake& operator=(Snake&& source); // move assignment operator

  // override functions
  void Initialize() override;
  void Update() override;
  bool Collide(const GameObject &other) override;

  // Actions
  void GrowBody();
  bool SnakeCell(int x, int y);
  void Shoot();

  // Attributes
  int size{1};
  bool alive{true};
  std::vector<std::shared_ptr<Bullet>> bullets;
  void enableShooterMode();

 private:
  void UpdateHead();
  void UpdateBullets();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
  std::thread threads;
  bool shooterMode{false};
  bool growing{false};

};

#endif
