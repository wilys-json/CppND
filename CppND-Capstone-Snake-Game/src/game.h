#ifndef GAME_H
#define GAME_H

#include <random>
#include <thread>
#include <deque>
#include <condition_variable>
#include <future>
#include <memory>
#include <vector>
#include <utility>
#include "SDL.h"
#include "map.h"
#include "food.h"
#include "player.h"
#include "controller.h"
#include "renderer.h"

template <class T>
class MessageQueue
{
public:
    void send(T&& msg);
    T receive();

private:
    std::deque<T> _queue;
    std::condition_variable _condition;
    std::mutex _mutex;
};

class Game : public std::enable_shared_from_this<Game>{
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Initialize();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

  // Owns the map
  std::shared_ptr<Map<GameObject>> map;

 private:
  std::vector<std::shared_ptr<GameObject>> objectPool;
  std::shared_ptr<PlayerSnake> player;  // owns Snake
  std::shared_ptr<Food> food;
  std::random_device dev;
  std::size_t grid_width;
  std::size_t grid_height;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  MessageQueue<std::shared_ptr<Snake>> foodConsumptionQueue;
  std::vector<std::thread> foodConsumptionThreads;
  int score{0};

  template <class T>
  void Place();
  void Update();
  void UpdateScore();
  void clearMap() { map->clear(); } ;
};

#endif
