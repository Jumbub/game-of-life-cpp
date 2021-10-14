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

struct LoopMeta {
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Texture* texture = nullptr;
  BoardMeta* board = nullptr;
};

LoopMeta setup() {
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);

  LoopMeta loop;

  loop.board = new BoardMeta(2560, 1440);
  benchmarkBoard(
      *loop.board, (uint)loop.board->width, (uint)loop.board->height);

  // Create window
  loop.window = SDL_CreateWindow(
      "Game of Speed", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      (int)loop.board->width, (int)loop.board->height, SDL_WINDOW_MAXIMIZED);
  loop.renderer = SDL_CreateRenderer(loop.window, -1, SDL_RENDERER_ACCELERATED);

  loop.texture = createTexture(
      loop.renderer, (int)loop.board->width, (int)loop.board->height);

  return loop;
}

void loop(LoopMeta loop, long maxComputations) {
  auto totalTimer = start();

  auto& window = loop.window;
  auto& renderer = loop.renderer;
  auto& texture = loop.texture;
  auto& board = *loop.board;

  std::mutex boardMutex;

  long computations = 0;
  long renders = 0;

  bool running = true;

  // Computation loop
  std::thread nextBoardThread(
      [&board, &running, &boardMutex, &computations, &maxComputations]() {
        while (running && computations < maxComputations) {
          std::this_thread::sleep_for(std::chrono::nanoseconds(1));
          std::scoped_lock gaurd(boardMutex);
          nextBoard(board);
          computations++;
          board.flip();
        }
      });

  // Render loop
  while (running && computations < maxComputations) {
    auto renderTimer = start();
    renderBoardSdl(board, renderer, texture);
    renders++;

    SDL_Event event;
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
        int width, height;
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
      } else if (
          event.type == SDL_KEYDOWN &&
          event.key.keysym.scancode == SDL_SCANCODE_H) {
        board.microsPerRender *= 2;
      } else if (
          event.type == SDL_KEYDOWN &&
          event.key.keysym.scancode == SDL_SCANCODE_L) {
        board.microsPerRender = std::max(board.microsPerRender/2, (uint)1);
      }
    }
    stopAndDelay(renderTimer, board.microsPerRender);
  }

  nextBoardThread.join();

  stopAndFps(totalTimer, renders, computations);
}

void shutdown(LoopMeta loop) {
  SDL_DestroyTexture(loop.texture);
  SDL_DestroyRenderer(loop.renderer);
  SDL_DestroyWindow(loop.window);
  SDL_Quit();

  delete loop.board;
}
