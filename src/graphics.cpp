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

    std::vector<SDL_Point> points;

    // Reset to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw white pixels
    for (int y = 0; y < height; ++y)
      for (int x = 0; x < width; ++x)
        if (board[y][x])
          points.push_back({x, y});
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoints(renderer, &points[0], points.size());

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
