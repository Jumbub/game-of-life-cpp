#include "../board/next.h"
#include "../board/random.h"
#include "../board/sdl.h"
#include "../util/profile.cpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <iostream>

int main() {
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Event event;

  // Create window
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_CreateWindowAndRenderer(500, 500, SDL_WINDOW_RESIZABLE, &window,
                              &renderer);

  // Generate initial board
  auto board = randomBoardForSdlWindow(window);

  bool running = true;
  while (running) {
    // Exit on escape key
    while (SDL_PollEvent(&event))
      if (event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
      else if (event.type == SDL_WINDOWEVENT_RESIZED)
        std::cout << "Resized window";
    /* board = randomBoardForSdlWindow(window); */

    board = nextBoard(board);
    renderBoardSdl(board, renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
