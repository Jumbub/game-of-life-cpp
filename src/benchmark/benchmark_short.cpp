#include "../../build/_deps/googlebenchmark-src/include/benchmark/benchmark.h"
#include "../board/generate.h"
#include "../board/loop.h"
#include "../board/next.h"

static void BM_Real(benchmark::State& state) {
  Loop loop(true);
  for (auto _ : state) {
    loop.run(500, state.range(0));
  }
}

BENCHMARK(BM_Real)
    ->Unit(benchmark::kSecond)
    ->MeasureProcessCPUTime()
    ->DenseRange(1, PROBABLY_OPTIMAL_THREAD_COUNT * 2, 1);

BENCHMARK_MAIN();
