#include <algorithm>
#include <cstring>
#include "../common/lock.h"
#include "board.h"

void Board::setOutputToInput() {
  std::swap(input, output);
}

void Board::setSize(const uint& width, const uint& height) {
  if (raw != nullptr && this->width * this->height != width * height) {
    delete[] raw;
    raw = nullptr;
  }

  this->width = width;
  this->height = height;

  this->rawWidth = width + PADDING;
  this->rawHeight = height + PADDING;
  this->rawSize = rawWidth * rawHeight;

  allocateBoardMemory(width, height);
}

void Board::allocateBoardMemory(const uint& width, const uint& height) {
  // Generate board with 1 cell of padding
  const uint size = (width + PADDING) * (height + PADDING);
  if (raw == nullptr) {
    raw = new Cell[size * 2];
    input = raw;
    output = input + size;
  }

  clearSkips();
}

void Board::clearSkips() {
  for (uint i = 0; i < rawSize * 2; i++) {
    resetSkip(raw[i]);
  }
}

Board::Board(const uint& width, const uint& height) {
  setSize(width, height);
}

Board::~Board() {
  delete[] raw;
}
