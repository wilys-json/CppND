#ifndef RIVAL_H
#define RIVAL_H
#include <thread>
#include "snake.h"
#include "map.h"


class RivalSnake : public Snake {
public:
  RivalSnake() {};
  RivalSnake(int grid_width, int grid_height, std::shared_ptr<Map<GameObject>> gameMap)
      : Snake(grid_width, grid_height, std::move(gameMap)),
        SensingRange(3) {};
  ~RivalSnake();

  void Shrink() override;
  void Update() override;
  void Initialize() override;
  void Initialize(const int& score, const int&x, const int& y);
  const Color getDefaultBodyColor() override;
  const Color getDefaultHeadColor() override;

private:
  enum class Mode { kBattle, kEscape, kAttack };
  Mode mode{Mode::kBattle};
  int SensingRange;
  std::mutex _walkMutex;
  std::thread walkThread;
  std::condition_variable walkConditionVariable;
  void Digest() override;
  void Move();
  void RandomWalk();
  void EnhanceSense(int i);
  void Sense();
  void Aim(const int& sensor_x, const int& sensor_y);
};

#endif
