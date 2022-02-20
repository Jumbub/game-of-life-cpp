#include <benchmark/benchmark.h>
#include "../common/setBenchmarkBoard.h"
#include "../graphics/loop.h"
#include "../logic/next.h"
#include "../logic/threads.h"

static void BM_DifferentThreadCounts(benchmark::State& state) {
  Loop loop(2560, 1440, "Game of Speed [Benchmark]", false);
  for (auto _ : state) {
    loop.run(2000, state.range(0), 1000000 / 30);
  }
}

BENCHMARK(BM_DifferentThreadCounts)
    ->Unit(benchmark::kSecond)
    ->MeasureProcessCPUTime()
    ->DenseRange(1, PROBABLY_OPTIMAL_THREAD_COUNT * 2, 1);

BENCHMARK_MAIN();
