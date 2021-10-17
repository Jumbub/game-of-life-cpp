#include <future>
#include <iostream>
#include <thread>
#include "../board/loop.h"

int main(int argc, char** args) {
  const long maxFrames = argc == 2 ? std::stoi(args[1]) : std::numeric_limits<long>::max();

  auto meta = setup();
  loop(meta, maxFrames);
  shutdown(meta);

  return EXIT_SUCCESS;
}
