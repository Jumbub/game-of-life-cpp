#pragma once

#include <SDL2/SDL.h>
#include <cstring>
#include <vector>
#include "board.h"
#include "generate.h"

SDL_Texture*
createTexture(SDL_Renderer* renderer, const int& width, const int& height) {
  return SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width,
      height);
}

void renderBoardSdl(
    BoardMeta& board,
    SDL_Window* window,
    int* pixels) {
  const auto& output = board.output;

  // I would LOVE it if someone could figure out how to create a "bool" pixel
  // format to use with SDL. Then I wouldn't need to do this memcpy trash.
  for (unsigned int i = 0; i < board.width * board.height; i++) {
    if (output[i] == ALIVE) {
      *pixels = INT32_MAX;
    } else {
      *pixels = 0;
    }
    pixels++;
  }

  SDL_UpdateWindowSurface(window);
}
