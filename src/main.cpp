#include "graphics/loop.h"
#include "logic/threads.h"

int main(int argc, char** argv) {
  const bool resizable = argc > 1 ? (bool)atoi(argv[1]) : true;
  const uint maxGenerations = argc > 2 ? (uint)atoi(argv[2]) : UINT32_MAX;
  const ulong rendersPerSecond = argc > 3 ? (uint)atoi(argv[3]) : 30;
  const uint workerThreads = argc > 4 ? (uint)atoi(argv[4]) : PROBABLY_OPTIMAL_THREAD_COUNT;

  Loop(2560, 1440, "Game of Speed", resizable).run(maxGenerations, workerThreads, 1000000 / rendersPerSecond);

  return EXIT_SUCCESS;
}
