#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <future>
#include <iostream>
#include <thread>
#include "../board/generate.h"
#include "../board/next.h"
#include "../board/sdl.h"
#include "../util/profile.h"

#ifndef ENABLE_THREADING
#define ENABLE_THREADING 1
#else
#undef ENABLE_THREADING
#endif

int main() {
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Event event;

  // Create window
  SDL_Window* window = SDL_CreateWindow(
      "Game of Speed", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 2560,
      1440, SDL_WINDOW_RESIZABLE);
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Window texture
  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  auto texture = createTexture(renderer, width, height);

  // Generate initial board
  BoardMeta board((unsigned int)width, (unsigned int)height);
  boardForSdlWindow(board, window);

  bool running = true;
  bool recreateBoard = false;
  while (running) {
    auto loopTimer = startProfiling();
    auto sdlTimer = startProfiling();

    // Start computing next board
    std::thread nextBoardThread([&board](){
      nextBoard(board);
    });

    while (SDL_PollEvent(&event)) {
      // Exit when told, or Escape is pressed
      if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ||
          (event.type == SDL_QUIT) ||
          (event.type == SDL_WINDOWEVENT &&
           event.window.event == SDL_WINDOWEVENT_CLOSE))
        running = false;
      // Re-create board when Enter is pressed, or window is resized
      else if (
          (event.type == SDL_KEYDOWN &&
           event.key.keysym.scancode == SDL_SCANCODE_RETURN) ||
          (event.type == SDL_WINDOWEVENT &&
           event.window.event == SDL_WINDOWEVENT_RESIZED)) {
        recreateBoard = true;
      } else if (
          event.type == SDL_KEYDOWN &&
          event.key.keysym.scancode == SDL_SCANCODE_J) {
        setThreads(getThreads() - 1);
        std::cout << "Setting thread count: " << getThreads() << std::endl;
      } else if (
          event.type == SDL_KEYDOWN &&
          event.key.keysym.scancode == SDL_SCANCODE_K) {
        setThreads(getThreads() + 1);
        std::cout << "Setting thread count: " << getThreads() << std::endl;
      }
    }

    renderBoardSdl(board, renderer, texture);
    stopProfiling(sdlTimer, "  sdl");

    // Wait for the board computation thread to complete
    auto joiningTimer = startProfiling();
    nextBoardThread.join();
    stopProfiling(joiningTimer, "  nextBoard.join");

    // Re-create board when computation is complete
    if (recreateBoard) {
      delete[] board.input;
      boardForSdlWindow(board, window);
      int width, height;
      SDL_GetWindowSize(window, &width, &height);
      SDL_DestroyTexture(texture);
      texture = createTexture(renderer, width, height);
      recreateBoard = false;
      std::cout << "Re-created board: " << width << "x" << height << std::endl;
    } else {
      std::swap(board.input, board.output);
    }

    stopProfiling(loopTimer, "main");
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
