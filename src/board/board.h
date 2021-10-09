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
  Cell* input = nullptr;
  Cell* output = nullptr;
  Cell* render = nullptr;

  uint width;
  uint height;

  Cell* raw = nullptr;
  Cell* rawRender = nullptr;

  uint threads;

  void flip() { std::swap(input, output); }

  void resize(const uint& width, const uint& height) {
    if (this->width * this->height != width * height) {
      delete[] raw;
      raw = nullptr;

      delete[] rawRender;
      rawRender = nullptr;
    }

    this->width = width;
    this->height = height;

    alloc(width, height);
  }

  void alloc(const uint& width, const uint& height) {
    const uint size = width * height;
    if (raw == nullptr) {
      raw = new Cell[size * 2];
      input = &raw[0];
      output = &raw[size];
    }
    if (rawRender == nullptr) {
      rawRender = new Cell[size];
      render = &rawRender[0];
    }
  }

  BoardMeta(const uint& width, const uint& height) {
    this->width = width;
    this->height = height;
    this->threads = PROBABLY_OPTIMAL_THREAD_COUNT;
    alloc(width, height);
  }

  ~BoardMeta() {
    delete[] raw;
    delete[] rawRender;
  }

 private:
  BoardMeta([[maybe_unused]] const BoardMeta& _){};
};
