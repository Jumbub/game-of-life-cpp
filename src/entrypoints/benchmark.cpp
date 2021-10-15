#include <SDL2/SDL.h>
#include <benchmark/benchmark.h>
#include "../board/generate.h"
#include "../board/loop.h"
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

BENCHMARK(BM_NextBoard)->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime()->UseRealTime()->Iterations(3000);

static void BM_RenderBoard(benchmark::State& state) {
  auto meta = setup();
  for (auto _ : state) {
    renderBoardSdl(*meta.board, meta.window, (int*)meta.surface->pixels);
  }
  shutdown(meta);
}
BENCHMARK(BM_RenderBoard)->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime()->UseRealTime();

static void BM_Main(benchmark::State& state) {
  auto meta = setup();
  for (auto _ : state) {
    loop(meta, 2000);
  }
  shutdown(meta);
}

BENCHMARK(BM_Main)->Unit(benchmark::kSecond)->MeasureProcessCPUTime()->UseRealTime()->Repetitions(1)->Iterations(1);

BENCHMARK_MAIN();
