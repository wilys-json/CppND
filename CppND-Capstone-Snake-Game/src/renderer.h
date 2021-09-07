#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include "SDL.h"
#include "food.h"
#include "player.h"
#include "SwitchableColor.h"
#include "bullet.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(std::vector<std::shared_ptr<GameObject>>& objectPool);
  void SetRenderDrawColor(const Color color);
  void SetRenderDrawColor(std::shared_ptr<Snake> snake);
  void UpdateWindowTitle(int score, int fps, int highest_score, int longest_playtime);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif
