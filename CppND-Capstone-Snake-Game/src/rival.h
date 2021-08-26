#ifndef RIVAL_H
#define RIVAL_H
#include "snake.h"


class RivalSnake : public Snake {
public:
  RivalSnake() {};
  RivalSnake(int grid_width, int grid_height, std::shared_ptr<Map<GameObject>> gameMap)
      : Snake(grid_width, grid_height, std::move(gameMap)) {};
  ~RivalSnake();

  void Move();
  void Update() override;
  const Color getDefaultBodyColor() override;
  const Color getDefaultHeadColor() override;

private:
  int SensingRange{3};
  void Digest() override;
};

#endif
