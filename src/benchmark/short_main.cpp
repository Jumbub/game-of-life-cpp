#include "../../build/_deps/googlebenchmark-src/include/benchmark/benchmark.h"
#include "../graphics/loop.h"
#include "../logic/generate.h"
#include "../logic/next.h"

static void BM_Real(benchmark::State& state) {
  Loop loop(2560, 1440, "Game of Speed (benchmark)", false);
  for (auto _ : state) {
    loop.run(500, state.range(0), 30);
  }
}

BENCHMARK(BM_Real)->Unit(benchmark::kSecond)->MeasureProcessCPUTime()->DenseRange(1, MAYBE_OPTIMAL_THREAD_COUNT * 2, 1);

BENCHMARK_MAIN();
