#ifndef SNAKE_H
#define SNAKE_H

#include <chrono>
#include <cmath>
#include <future>
#include "Movables.h"
#include "SwitchableColor.h"
#include "SDL.h"
#include "food.h"

class Bullet;

class Snake : public Movables, public GameObject {
 public:

  // Enum classes
  enum class State { kNormal, kShooter, kPoisoned, kSpeeding };

  // constructor
  Snake() {};
  Snake(int grid_width, int grid_height, std::shared_ptr<Map<GameObject>> gameMap)
      : GameObject(grid_width, grid_height, std::move(gameMap)),
        Movables(Direction::kUp, 0.1f) {}

  // destructor
  virtual ~Snake() {};

  // // Move & Copy Semantics
  // Snake(const Snake& source); // copy constructor
  // Snake& operator=(const Snake& source); // copy assignment operator
  // Snake(Snake&& source); // move constructor
  // Snake& operator=(Snake&& source); // move assignment operator

  // Implement GameObject & Movables
  void Initialize() override;
  bool Collide(const GameObject* other) override;
  virtual void Digest()=0;

  // Actions
  void GrowBody();
  // void Shoot();
  void Consume(std::shared_ptr<Food> food,
               std::promise<Food::State> prmFoodState);

  // Attributes & Containers
  int size{1};
  bool alive{true};
  std::vector<std::shared_ptr<Bullet>> bullets;

  // Getters
  State& getState() { return state; };
  virtual const Color getDefaultHeadColor() = 0;
  virtual const Color getDefaultBodyColor() = 0;

  // Setters
  void setRandomInt(const int& randomNumber);

  protected:
  // private actions
  void UpdateHead();
  // void UpdateBullets();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  // attributes & pointer
  State state{State::kNormal};
  bool growing{false};
  int RandomInt{0};
  std::shared_ptr<Food> foodConsumed;

};

#endif
