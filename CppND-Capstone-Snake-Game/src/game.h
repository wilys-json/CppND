#ifndef GAME_H
#define GAME_H

#include <random>
#include <thread>
#include <memory>
#include <vector>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "food.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameObject>>>> map;


 private:
  Snake snake;
  Food food;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  static std::vector<std::vector<std::shared_ptr<GameObject>>> empty_map;
  int score{0};

  void PlaceFood();
  void Update();
};

#endif
