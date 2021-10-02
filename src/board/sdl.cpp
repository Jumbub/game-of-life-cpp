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

  auto points = std::unique_ptr<SDL_Point[]>(new SDL_Point[width * height]);
  int count = 0;
  for (int i = 0; i < width * height; i++)
    if (input[i]) {
      points[count] = {i % width, i / width};
      count++;
    }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawPoints(renderer, &points[0], count);

  SDL_RenderPresent(renderer);
}
