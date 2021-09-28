#include "logic.cpp"
#include <benchmark/benchmark.h>

static void BM_NextBoard(benchmark::State &state) {
  srand(0);
  auto board = randomBoard(2560, 1440);

  for (auto _ : state) {
    board = nextBoard(board);
  }
}

BENCHMARK(BM_NextBoard)->Unit(benchmark::kMillisecond)->MinTime(10);

BENCHMARK_MAIN();
