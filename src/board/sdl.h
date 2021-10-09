#pragma once

#include <SDL2/SDL.h>
#include <cstring>
#include <vector>
#include "../util/profile.h"
#include "board.h"
#include "generate.h"

void boardForSdlWindow(BoardMeta &board, SDL_Window* window) {
  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  benchmarkBoard(board, (unsigned int)width, (unsigned int)height);
}

SDL_Texture*
createTexture(SDL_Renderer* renderer, const int& width, const int& height) {
  return SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width,
      height);
}

void renderBoardSdl(
    BoardMeta &board,
    SDL_Renderer* renderer,
    SDL_Texture* texture) {
  const auto& input = board.input;
  const auto& width = board.width;
  const auto& height = board.height;
  auto render = std::unique_ptr<Uint32[]>(new Uint32[width * height]);

  // I would LOVE it if someone could figure out how to create a "bool" pixel
  // format to use with SDL. Then I wouldn't need to do this memcpy trash.
  for (unsigned int i = 0; i < width * height; i++) {
    if (input[i] == ALIVE) {
      render[i] = UINT32_MAX;
    } else {
      render[i] = 0;
    }
  }

  static_assert(sizeof(render[0]) == sizeof(Uint32));
  SDL_UpdateTexture(
      texture, NULL, &render[0], (long unsigned int)width * sizeof(Uint32));

  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}
