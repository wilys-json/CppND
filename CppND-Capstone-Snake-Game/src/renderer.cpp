#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include "renderer.h"
#include "SwitchableColor.h"
#include "snake.h"
#include "food.h"
#include "bullet.h"

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

// void Renderer::Render(Snake const snake, Food const food) {
void Renderer::Render(std::vector<std::shared_ptr<GameObject>>& objectPool) {
  SDL_Rect block;

  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  for (auto &gameobject : objectPool) {

    if (*gameobject == gameClasses.FOOD) {
      std::shared_ptr<Food> food = std::dynamic_pointer_cast<Food>(gameobject);
      // Render food
      switch(food->getColor()) {
        case SwitchableColor::Color::kYellow:
          SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
          break;
        case SwitchableColor::Color::kWhite:
          SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
          break;
        case SwitchableColor::Color::kBlue:
          SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
          break;
        case SwitchableColor::Color::kRed:
          SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
          break;
      }
      block.x = food->origin_x * block.w;
      block.y = food->origin_y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
    }

    if (*gameobject == gameClasses.SNAKE) {
        std::shared_ptr<Snake> snake = std::dynamic_pointer_cast<Snake>(gameobject);
        // Render snake's body
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        for (SDL_Point const &point : snake->body) {
          block.x = point.x * block.w;
          block.y = point.y * block.h;
          SDL_RenderFillRect(sdl_renderer, &block);
        }

        // Render snake's head
        block.x = static_cast<int>(snake->origin_x) * block.w;
        block.y = static_cast<int>(snake->origin_y) * block.h;
        if (snake->alive) {
          SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
        } else {
          SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
        }
        SDL_RenderFillRect(sdl_renderer, &block);

        // Render bullet & bullet body

        if (!snake->bullets.empty()) {
          for (auto &bullet : snake->bullets) {
            if (bullet.get()!=nullptr) {
            block.x = static_cast<int>(bullet->origin_x) * block.w;
            block.y = static_cast<int>(bullet->origin_y) * block.h;
            switch(bullet->getColor()) {
              case SwitchableColor::Color::kYellow:
                SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
                break;
              case SwitchableColor::Color::kRed:
                SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
                break;
              }
            SDL_RenderFillRect(sdl_renderer, &block);
            }
          }
        }
    }

  }
  //
  // // Render food
  // switch(food.getColor()) {
  //   case SwitchableColor::Color::kYellow:
  //     SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  //     break;
  //   case SwitchableColor::Color::kWhite:
  //     SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  //     break;
  //   case SwitchableColor::Color::kBlue:
  //     SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  //     break;
  //   case SwitchableColor::Color::kRed:
  //     SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  //     break;
  // }
  // block.x = food.origin_x * block.w;
  // block.y = food.origin_y * block.h;
  // SDL_RenderFillRect(sdl_renderer, &block);
  //
  // // Render snake's body
  // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  // for (SDL_Point const &point : snake.body) {
  //   block.x = point.x * block.w;
  //   block.y = point.y * block.h;
  //   SDL_RenderFillRect(sdl_renderer, &block);
  // }
  //
  // // Render snake's head
  // block.x = static_cast<int>(snake.origin_x) * block.w;
  // block.y = static_cast<int>(snake.origin_y) * block.h;
  // if (snake.alive) {
  //   SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  // } else {
  //   SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  // }
  // SDL_RenderFillRect(sdl_renderer, &block);
  //
  // // Render bullet & bullet body
  //
  // if (!snake.bullets.empty()) {
  //   for (auto &bullet : snake.bullets) {
  //     if (bullet.get()!=nullptr) {
  //     block.x = static_cast<int>(bullet->origin_x) * block.w;
  //     block.y = static_cast<int>(bullet->origin_y) * block.h;
  //     switch(bullet->getColor()) {
  //       case SwitchableColor::Color::kYellow:
  //         SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  //         break;
  //       case SwitchableColor::Color::kRed:
  //         SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  //         break;
  //     }
  //     SDL_RenderFillRect(sdl_renderer, &block);
  //     }
  //   }
  // }


  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
