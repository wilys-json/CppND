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
#include <string>
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

  // Constructor & Destructor
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();

  // Methods
  void Initialize();
  void Run(Controller const &controller, Renderer &renderer, std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  int GetHighestScore() const;
  int GetLongestPlayTime() const;
  void Terminate();

  // Ownerships
  std::shared_ptr<Map<GameObject>> map;

 private:

  // Attributes & Pointers
  std::vector<std::shared_ptr<GameObject>> objectPool;
  std::shared_ptr<PlayerSnake> player;
  std::shared_ptr<Food> food;
  std::shared_ptr<RivalSnake> rival;
  std::random_device dev;
  std::size_t grid_width;
  std::size_t grid_height;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  MessageQueue<std::shared_ptr<Snake>> foodConsumptionQueue;
  int score{0};
  static const std::string cacheFilePath;
  struct Record {
    static const std::string highestScoreKeyWord;
    int HighestScore;
  } record;

  // Methods
  void readRecord();
  void writeRecord();
  void resetHighestScore();
  void Update();
  void CheckEvents();
  void clearMap() { map->clear(); } ;
  template <typename T>
  void Place();
};

#endif
