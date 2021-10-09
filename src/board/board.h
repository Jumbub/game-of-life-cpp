#pragma once

#include <array>
#include <iostream>
#include <memory>
#include <tuple>

using uint = unsigned int;
using Cell = uint;
[[maybe_unused]] const Cell ALIVE = 1;
[[maybe_unused]] const Cell DEAD = 0;

struct BoardMeta {
  Cell* input;
  Cell* output;
  uint width;
  uint height;

  Cell* raw;

  void flip() { std::swap(input, output); }

  void resize(const uint& width, const uint& height) {
    if (this->width*this->height != width * height) {
      delete[] raw;
      raw = nullptr;
    }

    this->width = width;
    this->height = height;

    if (raw == nullptr) {
      alloc(width, height);
    }
  }

  void alloc(const uint& width, const uint& height) {
    const uint size = width * height;
    raw = new Cell[size * 2];
    input = &raw[0];
    output = &raw[size];
  }

  BoardMeta(const uint& width, const uint& height) {
    this->width = width;
    this->height = height;
    alloc(width, height);
  }

  ~BoardMeta() { delete[] raw; }

 private:
  BoardMeta([[maybe_unused]] const BoardMeta& _){};
};
