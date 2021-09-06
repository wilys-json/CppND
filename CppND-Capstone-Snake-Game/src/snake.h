#ifndef SNAKE_H
#define SNAKE_H

#include <chrono>
#include <cmath>
#include <future>
#include <utility>
#include <memory>
#include "Movables.h"
#include "SwitchableColor.h"
#include "SDL.h"
#include "food.h"


class Snake : public Movables, public GameObject {
 public:

  // Enum classes
  enum class State { kNormal, kShooter, kPoisoned, kSpeeding };

  // constructor
  Snake() {};
  Snake(int grid_width, int grid_height, std::shared_ptr<Map<GameObject>> gameMap)
      : GameObject(grid_width, grid_height, std::move(gameMap)),
        Movables(Direction::kUp, 0.1f) {}

  // // destructor
  ~Snake() {};

  // // Move & Copy Semantics
  // Snake(const Snake& source); // copy constructor
  // Snake& operator=(const Snake& source); // copy assignment operator
  // Snake(Snake&& source); // move constructor
  // Snake& operator=(Snake&& source); // move assignment operator

  // Implement GameObject & Movables
  void Initialize() override;
  bool Collide(const GameObject* other) override;

  virtual void Digest()=0;
  virtual void Shrink()=0;

  // Actions
  void GrowBody();
  // void Shoot();
  void Consume(std::shared_ptr<Food> food,
               std::promise<std::shared_ptr<Snake>> prmFoodConsumption);

  // Attributes & Containers
  int size{1};
  bool alive{true};

  // Getters
  State& getState() { return state; };
  std::shared_ptr<Food> getFood() { return foodConsumed; };
  virtual const Color getDefaultHeadColor() = 0;
  virtual const Color getDefaultBodyColor() = 0;

  // Setters
  void setRandomInt(const int& randomNumber);
  void setRival(std::shared_ptr<Snake> rivalPtr);

  protected:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  // attributes & pointer
  State state{State::kNormal};
  bool growing{false};
  int RandomInt{0};
  std::shared_ptr<Food> foodConsumed;
  std::shared_ptr<Snake> rival;
  std::mutex _mutex;

};

#endif
