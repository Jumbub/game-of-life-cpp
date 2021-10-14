#include <SDL2/SDL.h>
#include <future>
#include <iostream>
#include <thread>
#include "../board/sdl.h"
#include "../board/loop.h"

int main(int argc, char **args) {
  const long maxFrames = argc == 2 ? std::stoi(args[1]) : LONG_MAX;

  auto meta = setup();
  loop(meta, maxFrames);
  shutdown(meta);

  return EXIT_SUCCESS;
}
