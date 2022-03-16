#pragma once

#include <boost/dynamic_bitset.hpp>
#include <cstdint>
#include "../common/lock.h"

using uint = unsigned int;
using ulong = unsigned long;

constexpr bool ALIVE = 1;
constexpr bool DEAD = 0;

constexpr bool COMPUTE = 1;
constexpr bool SKIP = 0;

constexpr unsigned int PADDING = 1;

struct Board {
  uint width;
  uint height;

  uint paddedSize;
  uint paddedWidth;
  uint paddedHeight;

  // TODO: investigate different block & allocator types
  boost::dynamic_bitset<> input;
  boost::dynamic_bitset<> output;
  boost::dynamic_bitset<> jobs;
  boost::dynamic_bitset<> nextJobs;

  Lock lock;

  void setOutputToInput();
  void setSize(const uint& width, const uint& height);
  void setJobs();
  Board(const uint& width, const uint& height);

 private:
  Board([[maybe_unused]] const Board& _){};
};
