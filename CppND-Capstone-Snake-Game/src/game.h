#ifndef GAME_H
#define GAME_H

#include <random>
#include <thread>
#include <memory>
#include <vector>
#include "SDL.h"
#include "gameclasses.h"

class Food;
class Snake;
class Controller;
class Renderer;
class GameObject;
class Map;

class Game : public std::enable_shared_from_this<Game>{
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Initialize(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

  // Owns the map
  std::shared_ptr<Map> map;

 private:
  GameClasses gameClasses;
  std::vector<std::shared_ptr<GameObject>> objectPool;
  std::shared_ptr<Snake> snake;  // owns Snake
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  int score{0};

  void PlaceFood();
  void Update();
  void clearMap() { map->clear(); } ;
};

#endif
