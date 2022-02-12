#include "../../build/_deps/googlebenchmark-src/include/benchmark/benchmark.h"
#include "../graphics/loop.h"
#include "../graphics/render.h"
#include "../logic/generate.h"
#include "../logic/next.h"

const unsigned int TEST_WIDTH = 2560;
const unsigned int TEST_HEIGHT = 1440;
const unsigned int TEST_RENDERS_PER_SECOND = 30;
const unsigned int TEST_GENERATIONS = 2000;

static void BM_GraphicsOnly(benchmark::State& state) {
  Loop loop(true);
  for (auto _ : state) {
    drawBoard(loop.board, loop.window, loop.sprite, loop.texture, loop.image, loop.pixels);
    loop.window.display();
  }
}
BENCHMARK(BM_GraphicsOnly)->Unit(benchmark::kMillisecond)->Iterations(100);

static void BM_LogicOnly(benchmark::State& state) {
  Board board(TEST_WIDTH, TEST_HEIGHT);
  assignBenchmarkCells(board);

  for (auto _ : state) {
    nextBoard(board, MAYBE_OPTIMAL_THREAD_COUNT);
  }
}

BENCHMARK(BM_LogicOnly)->Unit(benchmark::kMillisecond)->Repetitions(2)->Iterations(TEST_GENERATIONS);

static void BM_Main(benchmark::State& state) {
  Loop loop(true);
  for (auto _ : state) {
    loop.run(TEST_GENERATIONS, MAYBE_OPTIMAL_THREAD_COUNT, TEST_RENDERS_PER_SECOND);
  }
}

BENCHMARK(BM_Main)->Unit(benchmark::kSecond)->Repetitions(5)->Iterations(1);

BENCHMARK_MAIN();
