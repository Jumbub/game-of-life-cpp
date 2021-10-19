#include "../board/loop.h"

int main(int argc, char** argv) {
  Loop(argc > 2 ? (bool)atoi(argv[2]) : false).run(argc > 1 ? atol(argv[1]) : DEFAULT_MAX_GENERATIONS,PROBABLY_OPTIMAL_THREAD_COUNT,argc > 3 ? (uint)atoi(argv[3]) : 30);

  return EXIT_SUCCESS;
}
