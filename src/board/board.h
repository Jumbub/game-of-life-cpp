#pragma once

#include <cstring>
#include "../util/lock.h"

using Cell = uint8_t;
[[maybe_unused]] const Cell ALIVE = 1;
[[maybe_unused]] const Cell DEAD = 0;

struct Board {
  uint width;
  uint height;

  Cell* input = nullptr;
  Cell* output = nullptr;
  Cell* inSkip = nullptr;
  Cell* outSkip = nullptr;
  Cell* raw = nullptr;

  Lock lock;

  void setOutputToInput() {
    std::swap(input, output);
    std::swap(inSkip, outSkip);
    const uint size = (width + 2) * (height + 2);
    std::memset(outSkip, true, sizeof(Cell) * size);
  }

  void setSize(const uint& width, const uint& height) {
    if (this->width * this->height != width * height) {
      delete[] raw;
      raw = nullptr;
    }

    this->width = width;
    this->height = height;

    allocateBoardMemory(width, height);
  }

  void allocateBoardMemory(const uint& width, const uint& height) {
    // Generate board with 1 cell of padding
    const uint size = (width + 2) * (height + 2);
    if (raw == nullptr) {
      raw = new Cell[size * 4];
      std::memset(raw, 0, sizeof(Cell) * size * 4);
      input = raw;
      output = input + size;
      inSkip = output + size;
      outSkip = inSkip + size;
    }
  }

  void clearSkips() {
    const uint size = (width + 2) * (height + 2);
    std::memset(&raw[size * 2], false, size * 2);
  }

  Board(const uint& width, const uint& height) {
    this->width = width;
    this->height = height;
    allocateBoardMemory(width, height);
  }

  ~Board() { delete[] raw; }

 private:
  Board([[maybe_unused]] const Board& _){};
};
