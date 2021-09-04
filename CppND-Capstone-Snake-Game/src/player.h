#ifndef PLAYER_H
#define PLAYER_H
#include "snake.h"

class Bullet;

class PlayerSnake : public Snake {
public:
  PlayerSnake() {};
  PlayerSnake(int grid_width, int grid_height, std::shared_ptr<Map<GameObject>> gameMap)
      : Snake(grid_width, grid_height, std::move(gameMap)) {};
  ~PlayerSnake();

  void Update() override;
  void Shrink() override;
  bool Collide(const GameObject* other) override;
  const Color getDefaultBodyColor() override;
  const Color getDefaultHeadColor() override;

  void UpdateBullets();
  void Shoot();

  std::vector<std::shared_ptr<Bullet>> bullets;

private:
  void Digest() override;

};

#endif
