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
    raw = new Cell[size * 4];
    input = raw;
    output = &raw[1];
    inSkip = &raw[size * 2];
    outSkip = &raw[size * 3];
  }

  clearSkips();
}

void Board::clearSkips() {
  std::memset(&raw[rawSize * 2], false, rawSize * 2);
}

Board::Board(const uint& width, const uint& height) {
  setSize(width, height);
}

Board::~Board() {
  delete[] raw;
}
