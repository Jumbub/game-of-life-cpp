#include "../../../build/_deps/googlebenchmark-src/include/benchmark/benchmark.h"
#include "../board/generate.h"
#include "../board/loop.h"
#include "../board/next.h"

static void BM_DifferentThreadCounts(benchmark::State& state) {
  Loop loop(2560, 1440, "Game of Speed [Benchmark]", false);
  for (auto _ : state) {
    loop.run(2000, state.range(0), 30);
  }
}

BENCHMARK(BM_DifferentThreadCounts)
    ->Unit(benchmark::kSecond)
    ->MeasureProcessCPUTime()
    ->DenseRange(1, PROBABLY_OPTIMAL_THREAD_COUNT * 2, 1);

BENCHMARK_MAIN();
