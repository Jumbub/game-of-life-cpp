#pragma once

#include <boost/dynamic_bitset.hpp>
#include <cstdint>
#include "../common/lock.h"

using uint = unsigned int;
using ulong = unsigned long;

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
  boost::dynamic_bitset<> skips;
  boost::dynamic_bitset<> nextSkips;

  Lock lock;

  void setOutputToInput();
  void setSize(const uint& width, const uint& height);
  void clearSkips();
  Board(const uint& width, const uint& height);
  ~Board();

 private:
  Board([[maybe_unused]] const Board& _){};
};
