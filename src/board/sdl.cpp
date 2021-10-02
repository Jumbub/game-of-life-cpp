#include "sdl.h"
#include "../util/profile.h"
#include "generate.h"
#include <vector>

Board boardForSdlWindow(SDL_Window *window) {
  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  return benchmarkBoard(width, height);
}

void renderBoardSdl(Board board, SDL_Renderer *renderer, SDL_Texture *texture) {
  const auto &[input, width, height] = board;

  SDL_UpdateTexture(texture, NULL, &input[0], width * sizeof(Uint32));

  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}
