#include "../board/generate.h"
#include "../board/next.h"
#include "../board/sdl.h"
#include "../util/profile.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <iostream>
#include <thread>
#include <thread>
#include <future>

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
  SDL_CreateWindowAndRenderer(500, 500, SDL_WINDOW_RESIZABLE, &window,
                              &renderer);

  // Window texture
  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_STATIC, width, height);

  // Generate initial board
  auto board = boardForSdlWindow(window);

  bool running = true;
  bool recreateBoard = false;
  while (running) {
    auto loopTimer = startProfiling();

    std::promise<Board> nextBoardPromise;
    auto nextBoardFuture = nextBoardPromise.get_future();
    std::thread nextBoardThread(nextBoardThreaded, board, std::move(nextBoardPromise));

    while (SDL_PollEvent(&event)) {
      // Exit when told, or Escape is pressed
      if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ||
          (event.type == SDL_QUIT) ||
          (event.type == SDL_WINDOWEVENT &&
           event.window.event == SDL_WINDOWEVENT_CLOSE))
        running = false;
      // Re-create board when Enter is pressed
      else if (event.type == SDL_KEYDOWN &&
               event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
        recreateBoard = true;
      }
    }

    renderBoardSdl(board, renderer, texture);

    nextBoardThread.join();
    board = nextBoardFuture.get();

    // Re-create board when computation is complete
    if (recreateBoard) {
      free(get<0>(board));
      board = boardForSdlWindow(window);
      int width, height;
      SDL_GetWindowSize(window, &width, &height);
      SDL_DestroyTexture(texture);
      texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STATIC, width, height);
      recreateBoard = false;
    }

    stopProfiling(loopTimer, "Done loop");
  }

  free(get<0>(board));

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
