#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <memory>
#include <thread>
#include "Movables.h"
#include "SDL.h"

class Bullet;

class Snake : public Movables, public GameObject {
 public:
  // enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : GameObject(grid_width, grid_height),
        Movables(Direction::kUp, 0.1f) {}
  ~Snake() { bullets.clear(); };
  void Update() override;
  bool Collide(const GameObject &other) override;

  void GrowBody();
  bool SnakeCell(int x, int y);
  void Shoot();

  //
  // Direction direction = Direction::kUp;

  int size{1};
  bool alive{true};
  std::vector<std::shared_ptr<Bullet>> bullets;
  void enableShooterMode();
  // std::vector<Bullet*> bullets;
  // float head_x;
  // float head_y;
  // std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBullets();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
  std::thread threads;
  bool shooterMode{false};
  bool growing{false};


  // bool controllable{true};
  // int grid_width;
  // int grid_height;
};

#endif
