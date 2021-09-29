#include "sdl.h"
#include "random.h"

Board randomBoardForSdlWindow(SDL_Window *window) {
  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  return randomBoard(width, height);
}

void renderBoardSdl(Board board, SDL_Renderer *renderer) {
  std::vector<SDL_Point> points;
  for (int y = 0; y < (int)board.size(); ++y)
    for (int x = 0; x < (int)board[0].size(); ++x)
      if (board[y][x])
        points.push_back({x, y});

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawPoints(renderer, &points[0], points.size());

  SDL_RenderPresent(renderer);
}
