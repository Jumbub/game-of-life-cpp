#pragma once

#include <SDL2/SDL.h>
#include "board.h"

BoardMeta boardForSdlWindow(SDL_Window* window);

SDL_Texture*
createTexture(SDL_Renderer* renderer, const int& width, const int& height);

void renderBoardSdl(
    const BoardMeta board,
    SDL_Renderer* renderer,
    SDL_Texture* texture);
