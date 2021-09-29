#pragma once

#include "board.h"
#include <SDL2/SDL.h>

Board randomBoardForSdlWindow(SDL_Window *window);

void renderBoardSdl(Board board, SDL_Renderer *renderer);
