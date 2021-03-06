#include <benchmark/benchmark.h>
#include "../common/setBenchmarkBoard.h"
#include "../graphics/loop.h"
#include "../logic/next.h"
#include "../logic/threads.h"

static void BM_DifferentThreadCounts(benchmark::State& state) {
  Loop loop(2560, 1440, "Game of Life [Benchmark]", false);
  for (auto _ : state) {
    loop.run(2000, 4, state.range(0), 1000000 / 30);
  }
}

BENCHMARK(BM_DifferentThreadCounts)
    ->Unit(benchmark::kSecond)
    ->MeasureProcessCPUTime()
    ->DenseRange(2, PROBABLY_OPTIMAL_JOB_COUNT * 10, 2);

BENCHMARK_MAIN();
