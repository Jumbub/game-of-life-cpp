#pragma once

#include "../util/lock.h"

// State, neighbours
using Cell = uint8_t[2];
[[maybe_unused]] const uint8_t ALIVE = 1;
[[maybe_unused]] const uint8_t DEAD = 0;

struct Board {
  uint width;
  uint height;

  Cell* input = nullptr;
  Cell* output = nullptr;
  Cell* raw = nullptr;

  Lock lock;

  void setOutputToInput() { std::swap(input, output); }

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
      raw = new Cell[size * 2];
      input = &raw[0];
      output = &raw[size];
    }
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
