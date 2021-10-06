#include "sdl.h"
#include <vector>
#include <cstring>
#include "../util/profile.h"
#include "generate.h"

Board boardForSdlWindow(SDL_Window* window) {
  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  return benchmarkBoard(width, height);
}

SDL_Texture*
createTexture(SDL_Renderer* renderer, const int& width, const int& height) {
  return SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width,
      height);
}

void renderBoardSdl(
    const Board board,
    SDL_Renderer* renderer,
    SDL_Texture* texture) {
  const auto& [rawInput, width, height] = board;
  Cell* input = new Cell[width*height];

  // I would LOVE it if someone could figure out how to create a "bool" pixel
  // format to use with SDL. Then I wouldn't need to do this memcpy trash.
  std::memcpy(input, rawInput, width*height*sizeof(Cell));

  for (unsigned int i = 0; i < width * height; i ++) {
    if (input[i] == ALIVE) {
      input[i] = UINT32_MAX;
    }
  }

  static_assert(sizeof(input[0]) == sizeof(Uint32));
  SDL_UpdateTexture(
      texture, NULL, &input[0], (long unsigned int)width * sizeof(Uint32));

  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);

  delete[] input;
}
