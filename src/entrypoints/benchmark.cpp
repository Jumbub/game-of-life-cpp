#include "../board/generate.h"
#include "../board/next.h"
#include "../board/sdl.h"
#include <SDL2/SDL.h>
#include <benchmark/benchmark.h>

static void BM_NextBoard(benchmark::State &state) {
  const int width = 500;
  const int height = 500;

  auto board = benchmarkBoard(width, height);

  for (auto _ : state) {
    board = nextBoard(board);
  }
}

static void BM_RenderNextBoard(benchmark::State &state) {
  const int width = 500;
  const int height = 500;

  auto board = benchmarkBoard(width, height);

  // Initialize graphics
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

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
