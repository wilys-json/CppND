#ifndef SNAKE_H
#define SNAKE_H

#include <chrono>
#include <cmath>
#include "Movables.h"
#include "SDL.h"

class Bullet;
class Map;
class Food;

class Snake : public Movables, public GameObject {
 public:

  // Enum classes
  enum class State { kNormal, kShooter, kPoisoned, kSpeeding };

  // constructor
  Snake() {};
  Snake(int grid_width, int grid_height, std::shared_ptr<Map> gameMap)
      : GameObject(grid_width, grid_height, gameMap),
        Movables(Direction::kUp, 0.1f) {}

  // destructor
  ~Snake();

  // // Move & Copy Semantics
  // Snake(const Snake& source); // copy constructor
  // Snake& operator=(const Snake& source); // copy assignment operator
  // Snake(Snake&& source); // move constructor
  // Snake& operator=(Snake&& source); // move assignment operator

  // override functions
  void Initialize() override;
  void Update() override;
  bool Collide(const GameObject &other) override;

  // Actions
  void GrowBody();
  void Shoot();
  void enterShooterMode();
  void Digest();
  void Consume(std::shared_ptr<Food> &food);

  // Attributes
  int size{1};
  bool alive{true};
  std::vector<std::shared_ptr<Bullet>> bullets;

  // Getters
  State getState() { return state; };

  // Setters
  void setModeDuration(const int& randomNumber);

 private:
  void UpdateHead();
  void UpdateBullets();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
  State state{State::kNormal};
  bool growing{false};
  int ModeDuration{0};
  std::shared_ptr<Food> foodConsumed;

};

#endif
