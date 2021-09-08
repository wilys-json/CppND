#include "renderer.h"


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


void Renderer::Render(std::vector<std::shared_ptr<GameObject>>& objectPool) {
  SDL_Rect block;

  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  for (auto &gameobject : objectPool) {

    if (gameobject->isA<Food>()) {
      std::shared_ptr<Food> food = std::dynamic_pointer_cast<Food>(gameobject);
      SetRenderDrawColor(food->getColor());
      block.x = food->origin_x * block.w;
      block.y = food->origin_y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
      continue;
    }

    if (gameobject->isA<Snake>()) {
        std::shared_ptr<Snake> snake = std::dynamic_pointer_cast<Snake>(gameobject);
        if (snake->isA<PlayerSnake>()) {
          std::vector<std::shared_ptr<Bullet>> bullets = std::dynamic_pointer_cast<PlayerSnake>(snake)->bullets;
          if (!bullets.empty()) {
            for (auto &bullet : bullets) {
              if (bullet.get()!=nullptr) {
              block.x = static_cast<int>(bullet->origin_x) * block.w;
              block.y = static_cast<int>(bullet->origin_y) * block.h;
              SetRenderDrawColor(bullet->getColor());
              SDL_RenderFillRect(sdl_renderer, &block);
              }
            }
          }
        }


        // Render snake's body
        SetRenderDrawColor(snake->getDefaultBodyColor());
        for (SDL_Point const &point : snake->body) {
          block.x = point.x * block.w;
          block.y = point.y * block.h;
          SDL_RenderFillRect(sdl_renderer, &block);
        }

        // Render snake's head
        block.x = static_cast<int>(snake->origin_x) * block.w;
        block.y = static_cast<int>(snake->origin_y) * block.h;
        if (snake->alive) {
          SetRenderDrawColor(std::move(snake));
        } else {
          SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
        }

        SDL_RenderFillRect(sdl_renderer, &block);


      continue;
    }

  }
  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}


void Renderer::UpdateWindowTitle(int score, int fps,
    int highest_score) {
  std::string title{"Snake Score: " + std::to_string(score)
  + " FPS: " + std::to_string(fps)
  + " Highest Score: " + std::to_string(highest_score)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}


void Renderer::SetRenderDrawColor(const Color color) {
  switch(color) {
    case Color::kYellow:
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
      break;
    case Color::kWhite:
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
      break;
    case Color::kPurple:
      SDL_SetRenderDrawColor(sdl_renderer, 0x99, 0x00, 0xFF, 0xFF);
      break;
    case Color::kRed:
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
      break;
    case Color::kGreen:
      SDL_SetRenderDrawColor(sdl_renderer, 0x99, 0xFF, 0x99, 0xFF);
      break;
    case Color::kBlue:
      SDL_SetRenderDrawColor(sdl_renderer, 0x04, 0x65, 0xFA, 0xFF);
      break;
    case Color::kGrey:
      SDL_SetRenderDrawColor(sdl_renderer, 0x5A, 0x5A, 0x5A, 0xFF);
  }
}


void Renderer::SetRenderDrawColor(std::shared_ptr<Snake> snake) {
  switch(snake->getState()) {
    case Snake::State::kNormal:
      SetRenderDrawColor(snake->getDefaultHeadColor());
      break;
    case Snake::State::kShooter:
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
      break;
    case Snake::State::kPoisoned:
      SDL_SetRenderDrawColor(sdl_renderer, 0x99, 0x00, 0xFF, 0xFF);
      break;
    case Snake::State::kSpeeding:
      SDL_SetRenderDrawColor(sdl_renderer, 0x99, 0xFF, 0x99, 0xFF);
      break;
    }
}
