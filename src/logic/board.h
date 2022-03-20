#pragma once

#include <cstdint>
#include "../common/lock.h"

using uint = unsigned int;
using ulong = unsigned long;
using Cell = uint8_t;

const Cell SKIP_BIT = 1UL << 0;
const Cell ALIVE_BIT = 1UL << 1;

inline void setAlive(Cell& cell) {
  cell |= ALIVE_BIT;
}
inline void resetAlive(Cell& cell) {
  cell &= ~ALIVE_BIT;
}
inline bool testAlive(const Cell& cell) {
  return cell >> 1 & 1UL;
}

inline void setSkip(Cell& cell) {
  cell |= SKIP_BIT;
}
inline void resetSkip(Cell& cell) {
  cell &= ~SKIP_BIT;
}
inline bool testSkip(const Cell& cell) {
  return cell & 1;
}

constexpr unsigned int PADDING = 2;  // Padding of board left+right or top+bottom.

struct Board {
  uint rawSize;
  uint rawWidth;
  uint rawHeight;

  uint width;
  uint height;

  Cell* input = nullptr;
  Cell* output = nullptr;
  Cell* raw = nullptr;

  Lock lock;

  void setOutputToInput();
  void setSize(const uint& width, const uint& height);
  void clearSkips();
  Board(const uint& width, const uint& height);
  ~Board();

 private:
  void allocateBoardMemory(const uint& width, const uint& height);
  Board([[maybe_unused]] const Board& _){};
};
