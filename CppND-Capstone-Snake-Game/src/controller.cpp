#include <iostream>
#include "SDL.h"
#include "controller.h"

void Controller::ChangeDirection(std::shared_ptr<PlayerSnake> &player, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (player->direction != opposite || player->size == 1) player->direction = input;
  return;
}

void Controller::HandleInput(bool &running, std::shared_ptr<PlayerSnake> &player) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(player, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(player, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(player, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(player, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
        case SDLK_SPACE:
          player->Shoot();
          break;
      }
    }
  }
}
