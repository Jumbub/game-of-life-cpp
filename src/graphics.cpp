#include "logic.cpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <chrono>
#include <iostream>

Board randomBoard(int width, int height) {
  Board board(height, std::vector<bool>(width));
  for (int y = 0; y < 600; ++y) {
    for (int x = 0; x < 600; ++x) {
      board[y][x] = rand() % 2;
    }
  }
  return board;
}

int main() {
  const int width = 600;
  const int height = 600;

  SDL_Event event;
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

  auto begin = std::chrono::steady_clock::now();
  int frames = 0;
  int exit = 0;

  Board board = randomBoard(width, height);

  while (!exit) {
    for (int y = 0; y < 600; ++y)
      for (int x = 0; x < 600; ++x) {
        if (board[y][x])
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        else
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawPoint(renderer, x, y);
      }

    SDL_RenderPresent(renderer);

    board = nextBoard(board);

    while (SDL_PollEvent(&event))
      if (event.key.keysym.sym == SDLK_ESCAPE)
        exit = 1;

    frames++;
    if (frames > 60) {
      auto end = std::chrono::steady_clock::now();
      std::cout << "Microseconds per 60 frames: "
                << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                         begin)
                       .count()
                << std::endl;
      begin = std::chrono::steady_clock::now();
      frames = 0;
    }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;

  return 0;
}
