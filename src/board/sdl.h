#pragma once

#include "board.h"
#include <SDL2/SDL.h>

Board boardForSdlWindow(SDL_Window *window);

SDL_Texture *createTexture(SDL_Renderer *renderer, const int &width,
                           const int &height);

void renderBoardSdl(const Board board, SDL_Renderer *renderer,
                    SDL_Texture *texture);
