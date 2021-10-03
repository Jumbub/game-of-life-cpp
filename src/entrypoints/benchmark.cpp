#include "../board/generate.h"
#include "../board/next.h"
#include "../board/sdl.h"
#include <SDL2/SDL.h>
#include <benchmark/benchmark.h>

const int TEST_WIDTH = 2560;
const int TEST_HEIGHT = 1440;

static void BM_NextBoard(benchmark::State &state) {
  auto board = benchmarkBoard(TEST_WIDTH, TEST_HEIGHT);

  for (auto _ : state) {
    board = nextBoard(board);
  }

  delete get<0>(board);
}
BENCHMARK(BM_NextBoard)->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime()->UseRealTime();

static void BM_RenderBoard(benchmark::State &state) {
  auto board = benchmarkBoard(TEST_WIDTH, TEST_HEIGHT);

  // Initialize graphics
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_CreateWindowAndRenderer(TEST_WIDTH, TEST_HEIGHT, 0, &window, &renderer);
  auto texture = createTexture(renderer, TEST_WIDTH, TEST_HEIGHT);

  for (auto _ : state) {
    renderBoardSdl(board, renderer, texture);
  }

  // Destroy graphics
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  delete get<0>(board);
}
BENCHMARK(BM_RenderBoard)->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime()->UseRealTime();

BENCHMARK_MAIN();
