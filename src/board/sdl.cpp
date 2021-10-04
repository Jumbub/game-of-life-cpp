#include "sdl.h"
#include "../util/profile.h"
#include "generate.h"
#include <vector>

Board boardForSdlWindow(SDL_Window *window) {
  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  return benchmarkBoard(width, height);
}

SDL_Texture *createTexture(SDL_Renderer *renderer, const int &width,
                           const int &height) {
  return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                           SDL_TEXTUREACCESS_STATIC, width, height);
}

void renderBoardSdl(const Board board, SDL_Renderer *renderer, SDL_Texture *texture) {
  const auto &[input, width, height] = board;

  static_assert(sizeof(input[0]) == sizeof(Uint32));
  SDL_UpdateTexture(texture, NULL, &input[0], (long unsigned int)width * sizeof(Uint32));

  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}
