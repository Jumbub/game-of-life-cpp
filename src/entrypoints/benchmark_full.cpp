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
  board.threads = state.range(0);

  for (auto _ : state) {
    nextBoard(board);
    board.flip();
  }
}

BENCHMARK(BM_NextBoard)
    ->Unit(benchmark::kMillisecond)
    ->MeasureProcessCPUTime()
    ->MinTime(3)
    ->DenseRange(1, PROBABLY_OPTIMAL_THREAD_COUNT*2, 1);

BENCHMARK_MAIN();
