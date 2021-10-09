#include <SDL2/SDL.h>
#include <benchmark/benchmark.h>
#include "../board/generate.h"
#include "../board/next.h"
#include "../board/sdl.h"

const unsigned int TEST_WIDTH = 2560;
const unsigned int TEST_HEIGHT = 1440;

static void BM_NextBoard(benchmark::State& state) {
  BoardMeta board(TEST_WIDTH, TEST_HEIGHT);
  benchmarkBoard(board, TEST_WIDTH, TEST_HEIGHT);

  for (auto _ : state) {
    nextBoard(board);
    board.flip();
  }
}

BENCHMARK(BM_NextBoard)
    ->Unit(benchmark::kMillisecond)
    ->MeasureProcessCPUTime()
    ->UseRealTime()
    ->MinTime(10);

static void BM_RenderBoard(benchmark::State& state) {
  BoardMeta board(TEST_WIDTH, TEST_HEIGHT);
  benchmarkBoard(board, TEST_WIDTH, TEST_HEIGHT);

  // Initialize graphics
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Renderer* renderer;
  SDL_Window* window;
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
}
BENCHMARK(BM_RenderBoard)
    ->Unit(benchmark::kMillisecond)
    ->MeasureProcessCPUTime()
    ->UseRealTime();

BENCHMARK_MAIN();
