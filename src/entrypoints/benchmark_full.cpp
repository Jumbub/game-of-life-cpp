#include <SDL2/SDL.h>
#include <benchmark/benchmark.h>
#include "../board/generate.h"
#include "../board/next.h"
#include "../board/sdl.h"
#include "../board/loop.h"

static void BM_Real(benchmark::State& state) {
  auto meta = setup();
  for (auto _ : state) {
    loop(meta, 1000);
  }
  shutdown(meta);
}

BENCHMARK(BM_Real)
    ->Unit(benchmark::kSecond)
    ->MeasureProcessCPUTime()
    ->MinTime(3)
    ->DenseRange(1, PROBABLY_OPTIMAL_THREAD_COUNT*2, 1);

BENCHMARK_MAIN();
