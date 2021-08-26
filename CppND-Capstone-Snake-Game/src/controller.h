#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include "player.h"

class Controller {
 public:
  void HandleInput(bool &running, std::shared_ptr<PlayerSnake> &player) const;

 private:
  void ChangeDirection(std::shared_ptr<PlayerSnake> &player, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif
