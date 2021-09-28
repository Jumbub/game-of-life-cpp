#include "logic.cpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <chrono>
#include <iostream>

using milliseconds = std::chrono::milliseconds;

void tilNow(std::string label,
            std::chrono::time_point<std::chrono::steady_clock> start) {
  std::cout << label << " time: "
            << std::chrono::duration_cast<milliseconds>(
                   std::chrono::steady_clock::now() - start)
                   .count()
            << " ms" << std::endl;
}

int main() {
  const int width = 600;
  const int height = 600;

  SDL_Event event;
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

  Board board = randomBoard(width, height);

  bool exit = false;
  while (!exit) {
    auto startSdl = std::chrono::steady_clock::now();

    for (int y = 0; y < 600; ++y)
      for (int x = 0; x < 600; ++x) {
        if (board[y][x])
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        else
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawPoint(renderer, x, y);
      }

    SDL_RenderPresent(renderer);
    while (SDL_PollEvent(&event))
      if (event.key.keysym.sym == SDLK_ESCAPE)
        exit = true;

    tilNow("SDL", startSdl);

    auto startBoard = std::chrono::steady_clock::now();
    board = nextBoard(board);
    tilNow("Board computation", startBoard);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
