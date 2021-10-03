#include "../board/generate.h"
#include "../board/next.h"
#include "../board/sdl.h"
#include "../util/profile.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <future>
#include <iostream>
#include <thread>

#ifndef ENABLE_THREADING
#define ENABLE_THREADING 1
#else
#undef ENABLE_THREADING
#endif

void nextBoardThreaded(Board board, std::promise<Board> promise) {
  promise.set_value(nextBoard(board));
}

int main() {
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Event event;

  // Create window
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_CreateWindowAndRenderer(2560, 1440, SDL_WINDOW_RESIZABLE, &window,
                              &renderer);

  // Window texture
  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  auto texture = createTexture(renderer, width, height);

  // Generate initial board
  auto board = boardForSdlWindow(window);

  bool running = true;
  bool recreateBoard = false;
  while (running) {
    /* auto loopTimer = startProfiling(); */

#ifdef ENABLE_THREADING
    std::promise<Board> nextBoardPromise;
    auto nextBoardFuture = nextBoardPromise.get_future();
    std::thread nextBoardThread(nextBoardThreaded, board,
                                std::move(nextBoardPromise));
#endif

    while (SDL_PollEvent(&event)) {
      // Exit when told, or Escape is pressed
      if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ||
          (event.type == SDL_QUIT) ||
          (event.type == SDL_WINDOWEVENT &&
           event.window.event == SDL_WINDOWEVENT_CLOSE))
        running = false;
      // Re-create board when Enter is pressed, or window is resized
      else if ((event.type == SDL_KEYDOWN &&
            event.key.keysym.scancode == SDL_SCANCODE_RETURN) || (event.type == SDL_WINDOWEVENT &&
           event.window.event == SDL_WINDOWEVENT_RESIZED)) {
        recreateBoard = true;
      } else if (event.type == SDL_KEYDOWN &&
               event.key.keysym.scancode == SDL_SCANCODE_J) {
        setThreads(getThreads()-1);
        std::cout << "Setting thread count: " << getThreads() << std::endl;
      } else if (event.type == SDL_KEYDOWN &&
               event.key.keysym.scancode == SDL_SCANCODE_K) {
        setThreads(getThreads()+1);
        std::cout << "Setting thread count: " << getThreads() << std::endl;
      }
    }

    renderBoardSdl(board, renderer, texture);

#ifdef ENABLE_THREADING
    nextBoardThread.join();
    board = nextBoardFuture.get();
#else
    board = nextBoard(board);
#endif

    // Re-create board when computation is complete
    if (recreateBoard) {
      free(get<0>(board));
      board = boardForSdlWindow(window);
      int width, height;
      SDL_GetWindowSize(window, &width, &height);
      SDL_DestroyTexture(texture);
      texture = createTexture(renderer, width, height);
      recreateBoard = false;
      std::cout << "Re-created board: " << width << "x" << height << std::endl;
    }

    /* stopProfiling(loopTimer, "Done loop"); */
  }

  free(get<0>(board));

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
