#pragma once

#include <bitset>
#include <cstdint>
#include "../common/lock.h"

using uint = unsigned int;
using ulong = unsigned long;

constexpr bool ALIVE = 1;
constexpr bool DEAD = 0;

constexpr bool DONT_SKIP = 1;
constexpr bool SKIP = 0;

constexpr unsigned int PADDING = 1;

struct Board {
  uint width;
  uint height;

  uint paddedSize;
  uint paddedWidth;
  uint paddedHeight;

  // TODO: investigate different block & allocator types
  std::bitset<2562 * 1442> input;
  std::bitset<2562 * 1442> output;
  std::bitset<2562 * 1442> jobs;
  std::bitset<2562 * 1442> nextJobs;

  Lock lock;

  void setOutputToInput();
  void setSize(const uint& width, const uint& height);
  void setJobs();
  Board(const uint& width, const uint& height);

 private:
  Board([[maybe_unused]] const Board& _){};
};
