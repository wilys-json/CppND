#ifndef RIVAL_H
#define RIVAL_H
#include "snake.h"
#include "map.h"


class RivalSnake : public Snake {
public:
  RivalSnake() {};
  RivalSnake(int grid_width, int grid_height, std::shared_ptr<Map<GameObject>> gameMap)
      : Snake(grid_width, grid_height, std::move(gameMap)),
        SensingRange(3) {};
  ~RivalSnake();

  void Move();
  void Update() override;
  void Initialize() override;
  const Color getDefaultBodyColor() override;
  const Color getDefaultHeadColor() override;

private:
  enum class Mode { kBattle, kEscape, kAttack };
  Mode mode{Mode::kBattle};
  int SensingRange;
  // std::unique_ptr<Map<GameObject>> sensorMap{};
  std::mutex _walkMutex;
  void Digest() override;
  void RandomWalk();
  // void InitializeSensorMap();
  bool Sense();
  void Aim(const int& sensor_x, const int& sensor_y);
};

#endif
