#include <benchmark/benchmark.h>
#include "../board/generate.h"
#include "../board/loop.h"
#include "../board/next.h"

static void BM_Real(benchmark::State& state) {
  auto meta = setup();
  meta.board->threads = state.range(0);
  for (auto _ : state) {
    loop(meta, 500);
  }
  shutdown(meta);
}

BENCHMARK(BM_Real)
    ->Unit(benchmark::kSecond)
    ->MeasureProcessCPUTime()
    ->DenseRange(1, PROBABLY_OPTIMAL_THREAD_COUNT * 2, 1);

BENCHMARK_MAIN();
