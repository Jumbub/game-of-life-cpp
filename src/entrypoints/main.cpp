#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <future>
#include <iostream>
#include <thread>
#include "../board/generate.h"
#include "../board/next.h"
#include "../board/sdl.h"
#include "../util/time.h"

using namespace std::chrono;

constexpr long MICROS_PER_RENDER = 7000; // ~144fps

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
  BoardMeta board((uint)width, (uint)height);
  benchmarkBoard(board, (uint)width, (uint)height);

  // Global variables
  bool running = true;
  std::mutex boardMutex;

  // Computation loop
  std::thread nextBoardThread([&board, &running, &boardMutex]() {
    while (running) {
      auto timer = start();
      std::this_thread::sleep_for(std::chrono::nanoseconds(1));
      std::scoped_lock gaurd(boardMutex);
      nextBoard(board);
      board.flip();
      stop("Logic loop", timer);
    }
  });

  // Render loop
  while (running) {
    auto timer = start();

    renderBoardSdl(board, renderer, texture);

    while (SDL_PollEvent(&event)) {
      // Exit when told, or Escape is pressed
      if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ||
          (event.type == SDL_QUIT) ||
          (event.type == SDL_WINDOWEVENT &&
           event.window.event == SDL_WINDOWEVENT_CLOSE)) {
        running = false;
      } else if (
          (event.type == SDL_KEYDOWN &&
           event.key.keysym.scancode == SDL_SCANCODE_RETURN) ||
          (event.type == SDL_WINDOWEVENT &&
           event.window.event == SDL_WINDOWEVENT_RESIZED)) {
        std::scoped_lock gaurd(boardMutex);
        SDL_GetWindowSize(window, &width, &height);
        benchmarkBoard(board, (uint)width, (uint)height);
        SDL_DestroyTexture(texture);
        texture = createTexture(renderer, width, height);
      } else if (
          event.type == SDL_KEYDOWN &&
          event.key.keysym.scancode == SDL_SCANCODE_J) {
        std::scoped_lock gaurd(boardMutex);
        board.threads = std::max(board.threads - 1, (uint)1);
      } else if (
          event.type == SDL_KEYDOWN &&
          event.key.keysym.scancode == SDL_SCANCODE_K) {
        std::scoped_lock gaurd(boardMutex);
        board.threads++;
      }
    }

    stopAndDelay("Render loop", MICROS_PER_RENDER, timer);
  }

  nextBoardThread.join();

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
