#include <benchmark/benchmark.h>
#include "../common/setBenchmarkBoard.h"
#include "../graphics/loop.h"
#include "../graphics/renderBoard.h"
#include "../logic/next.h"
#include "../logic/threads.h"

const unsigned int TEST_WIDTH = 2560;
const unsigned int TEST_HEIGHT = 1440;
const std::string TEST_TITLE = "Game of Life [Benchmark]";
const bool TEST_RESIZABLE = false;

static void BM_NextBoard(benchmark::State& state) {
  Board board(TEST_WIDTH, TEST_HEIGHT);
  setBenchmarkBoard(board);

  for (auto _ : state) {
    nextBoard(board, PROBABLY_OPTIMAL_THREAD_COUNT, PROBABLY_OPTIMAL_JOB_COUNT);
  }
}

BENCHMARK(BM_NextBoard)->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime()->UseRealTime()->Iterations(3000);

static void BM_RenderBoard(benchmark::State& state) {
  Loop loop(TEST_WIDTH, TEST_HEIGHT, TEST_TITLE, TEST_RESIZABLE);
  for (auto _ : state) {
    renderBoard(loop.board, loop.window, loop.sprite, loop.texture, loop.pixels);
    loop.window.display();
  }
}
BENCHMARK(BM_RenderBoard)->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime()->UseRealTime();

static void BM_Main(benchmark::State& state) {
  Loop loop(TEST_WIDTH, TEST_HEIGHT, TEST_TITLE, TEST_RESIZABLE);
  for (auto _ : state) {
    loop.run(2000, PROBABLY_OPTIMAL_THREAD_COUNT, PROBABLY_OPTIMAL_JOB_COUNT, 1000000 / 30);
  }
}

BENCHMARK(BM_Main)->Unit(benchmark::kSecond)->MeasureProcessCPUTime()->UseRealTime()->Repetitions(3)->Iterations(1);

BENCHMARK_MAIN();
