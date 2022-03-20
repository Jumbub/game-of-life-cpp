#pragma once

#include <cstdint>
#include "../common/lock.h"

using uint = unsigned int;
using ulong = unsigned long;
using Cell = uint8_t;

const Cell ALIVE = 1;
const Cell DEAD = 0;

const uint8_t SKIPS_PER_BYTE = 2;
const uint8_t SKIP_BYTE = 1 + 2;
const uint8_t NO_SKIP_BYTE = 0;

const uint64_t SKIP_EIGHT_BYTES = ((ulong)SKIP_BYTE << 0) + ((ulong)SKIP_BYTE << 8) + ((ulong)SKIP_BYTE << 16) +
                                  ((ulong)SKIP_BYTE << 24) + ((ulong)SKIP_BYTE << 32) + ((ulong)SKIP_BYTE << 40) +
                                  ((ulong)SKIP_BYTE << 48) + ((ulong)SKIP_BYTE << 56);

constexpr unsigned int PADDING = 2;  // Padding of board left+right or top+bottom.

struct Board {
  uint rawSize;
  uint rawWidth;
  uint rawHeight;

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
  void clearSkips();
  Board(const uint& width, const uint& height);
  ~Board();

 private:
  Board([[maybe_unused]] const Board& _){};
};
