#ifndef RIVAL_H
#define RIVAL_H
#include <thread>
#include "snake.h"
#include "map.h"


class RivalSnake : public Snake {
public:
  // Constructors
  RivalSnake() {};
  RivalSnake(int grid_width, int grid_height, std::shared_ptr<Map<GameObject>> gameMap)
      : Snake(grid_width, grid_height, std::move(gameMap)),
        SensingRange(3) {};

  // Destructor
  ~RivalSnake();

  // Implement Abstract Classes
  void Shrink() override;
  void Update() override;
  void Initialize() override;
  void Initialize(const int& score, const int&x, const int& y);
  const Color getDefaultBodyColor() override;
  const Color getDefaultHeadColor() override;

private:
  // Enums for Mode switching
  enum class Mode { kBattle, kEscape, kAttack };

  // Attributes
  Mode mode{Mode::kBattle};
  int SensingRange;
  std::mutex _walkMutex;
  std::thread walkThread;
  std::condition_variable walkConditionVariable;

  // Implement `Snake`
  void Digest() override;

  // Methods
  void RandomWalk();
  void EnhanceSense(int i);
  void Sense();
  void Aim(const int& sensor_x, const int& sensor_y);
};

#endif
