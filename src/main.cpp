#include "graphics/loop.h"

int main(int argc, char** argv) {
  const auto resizable = argc > 1 ? (bool)atoi(argv[1]) : true;
  const auto maxGenerations = argc > 2 ? (uint)atoi(argv[2]) : UINT32_MAX;
  const auto rendersPerSecond = argc > 3 ? (uint)atoi(argv[3]) : 30;
  const auto workerThreads = argc > 4 ? (uint)atoi(argv[4]) : MAYBE_OPTIMAL_THREAD_COUNT;

  Loop(2560, 1440, "Game of Speed", resizable).run(maxGenerations, workerThreads, rendersPerSecond);

  return EXIT_SUCCESS;
}
