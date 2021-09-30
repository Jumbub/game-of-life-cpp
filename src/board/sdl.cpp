#include "sdl.h"
#include "generate.h"
#include <vector>

Board boardForSdlWindow(SDL_Window *window) {
  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  return benchmarkBoard(width, height);
}

void renderBoardSdl(Board board, SDL_Renderer *renderer) {
  const auto &[input, width, height] = board;

  std::vector<SDL_Point> points;
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
      if (input[y * width + x])
        points.push_back({x, y});

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawPoints(renderer, &points[0], points.size());

  SDL_RenderPresent(renderer);
}
