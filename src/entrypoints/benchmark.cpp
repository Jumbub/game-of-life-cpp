#include "../board/next.h"
#include "../board/random.h"
#include "../board/sdl.h"
#include <SDL2/SDL.h>
#include <benchmark/benchmark.h>

const int WIDTH = 500;
const int HEIGHT = 500;

static void BM_NextBoard(benchmark::State &state) {
  srand(0);
  auto board = randomBoard(WIDTH, HEIGHT);

  for (auto _ : state) {
    board = nextBoard(board);
  }
}

static void BM_RenderNextBoard(benchmark::State &state) {
  srand(0);
  auto board = randomBoard(WIDTH, HEIGHT);

  // Initialize graphics
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

  for (auto _ : state) {
    board = nextBoard(board);
    renderBoardSdl(board, renderer);
  }

  // Destroy graphics
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

BENCHMARK(BM_NextBoard)->Unit(benchmark::kMillisecond)->MinTime(5);
BENCHMARK(BM_RenderNextBoard)->Unit(benchmark::kMillisecond)->MinTime(5);

BENCHMARK_MAIN();
