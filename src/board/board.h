#pragma once

#include <array>
#include <iostream>
#include <memory>
#include <thread>
#include <tuple>
#include "threads.h"

using uint = unsigned int;
using Cell = uint;
[[maybe_unused]] const Cell ALIVE = 1;
[[maybe_unused]] const Cell ALIVE_RENDER = UINT32_MAX;
[[maybe_unused]] const Cell DEAD = 0;
[[maybe_unused]] const Cell DEAD_RENDER = 0;

struct BoardMeta {
  uint width;
  uint height;

  uint rawWidth;
  uint rawHeight;

  Cell* input = nullptr;
  Cell* output = nullptr;
  Cell* raw = nullptr;

  uint threads = PROBABLY_OPTIMAL_THREAD_COUNT;
  uint microsPerRender = 32000; // 30fps

  void flip() { std::swap(input, output); }

  void resize(const uint& width, const uint& height) {
    if (this->width * this->height != width * height) {
      delete[] raw;
      raw = nullptr;
    }

    this->width = width;
    this->height = height;

    alloc(width, height);
  }

  void alloc(const uint& width, const uint& height) {
    const uint size = (width+2)*(height+2);
    if (raw == nullptr) {
      raw = new Cell[size*2];
      input = &raw[0];
      output = &raw[size];
    }
  }

  BoardMeta(const uint& width, const uint& height) {
    this->width = width;
    this->height = height;
    alloc(width, height);
  }

  ~BoardMeta() {
    delete[] raw;
  }

 private:
  BoardMeta([[maybe_unused]] const BoardMeta& _){};
};
