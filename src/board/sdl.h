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
    SDL_Renderer* renderer,
    SDL_Texture* texture) {
  const auto& render = board.render;
  const auto& width = board.width;

  static_assert(sizeof(render[0]) == sizeof(Uint32));
  SDL_UpdateTexture(texture, NULL, &render[0], (int)(width * sizeof(Uint32)));
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}
