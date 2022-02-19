#pragma once

#include <cstdint>
#include "../util/lock.h"

using uint = unsigned int;
using Cell = uint8_t;
[[maybe_unused]] const Cell ALIVE = 1;
[[maybe_unused]] const Cell DEAD = 0;

constexpr unsigned int PADDING = 2;  // Padding of board left+right or top+bottom.

struct Board {
  uint width;
  uint height;

  Cell* input = nullptr;
  Cell* output = nullptr;
  Cell* inSkip = nullptr;
  Cell* outSkip = nullptr;
  Cell* raw = nullptr;

  Lock lock;

  void setOutputToInput();
  void setSize(const uint& width, const uint& height);
  void allocateBoardMemory(const uint& width, const uint& height);
  void clearSkips();
  Board(const uint& width, const uint& height);
  ~Board();

 private:
  Board([[maybe_unused]] const Board& _){};
};
