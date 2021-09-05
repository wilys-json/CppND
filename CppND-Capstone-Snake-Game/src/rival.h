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


  void Shrink();
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
  void Digest() override;
  void Move();
  void RandomWalk();
  bool Sense();
  void EnhanceSense(int i);
  void Aim(const int& sensor_x, const int& sensor_y);
};

#endif
