#include <algorithm>
#include <cstring>
#include "../common/lock.h"
#include "board.h"

void Board::setOutputToInput() {
  std::swap(input, output);
  std::swap(inSkip, outSkip);
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
    // (size + 1) for skip final values
    raw = new Cell[size * 2 + (size + 1) * 2];
    input = raw;
    output = input + size;
    inSkip = output + size;
    outSkip = inSkip + 1 + size;
  }

  clearSkips();
}

void Board::clearSkips() {
  std::memset(&raw[rawSize * 2], false, (rawSize + 1) * 2);
}

Board::Board(const uint& width, const uint& height) {
  setSize(width, height);
  allocateBoardMemory(width, height);
}

Board::~Board() {
  delete[] raw;
}
