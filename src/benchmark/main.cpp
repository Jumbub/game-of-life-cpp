#include <benchmark/benchmark.h>
#include "../benchmark/generate.h"
#include "../graphics/loop.h"
#include "../graphics/render.h"
#include "../logic/next.h"

const unsigned int TEST_WIDTH = 2560;
const unsigned int TEST_HEIGHT = 1440;
const std::string TEST_TITLE = "Game of Speed [Benchmark]";
const bool TEST_RESIZABLE = false;

static void BM_NextBoard(benchmark::State& state) {
  Board board(TEST_WIDTH, TEST_HEIGHT);
  assignBenchmarkCells(board);

  for (auto _ : state) {
    nextBoard(board, PROBABLY_OPTIMAL_THREAD_COUNT);
  }
}

BENCHMARK(BM_NextBoard)->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime()->UseRealTime()->Iterations(3000);

static void BM_RenderBoard(benchmark::State& state) {
  Loop loop(TEST_WIDTH, TEST_HEIGHT, TEST_TITLE, TEST_RESIZABLE);
  for (auto _ : state) {
    drawBoard(loop.board, loop.window, loop.sprite, loop.texture, loop.image, loop.pixels);
    loop.window.display();
  }
}
BENCHMARK(BM_RenderBoard)->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime()->UseRealTime();

static void BM_Main(benchmark::State& state) {
  Loop loop(TEST_WIDTH, TEST_HEIGHT, TEST_TITLE, TEST_RESIZABLE);
  for (auto _ : state) {
    loop.run(2000, PROBABLY_OPTIMAL_THREAD_COUNT, 30);
  }
}

BENCHMARK(BM_Main)->Unit(benchmark::kSecond)->MeasureProcessCPUTime()->UseRealTime()->Repetitions(3)->Iterations(1);

BENCHMARK_MAIN();
