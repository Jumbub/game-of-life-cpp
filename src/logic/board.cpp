#include <algorithm>
#include <cstring>
#include "../util/lock.h"
#include "board.h"

void Board::setOutputToInput() {
  std::swap(input, output);
  std::swap(inSkip, outSkip);
}

void Board::setSize(const uint& width, const uint& height) {
  if (this->width * this->height != width * height) {
    delete[] raw;
    raw = nullptr;
  }

  this->width = width;
  this->height = height;

  allocateBoardMemory(width, height);
}

void Board::allocateBoardMemory(const uint& width, const uint& height) {
  // Generate board with 1 cell of padding
  const uint size = (width + 2) * (height + 2);
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
  const uint size = (width + 2) * (height + 2);
  std::memset(&raw[size * 2], false, (size + 1) * 2);
}

Board::Board(const uint& width, const uint& height) {
  this->width = width;
  this->height = height;
  allocateBoardMemory(width, height);
}

Board::~Board() {
  delete[] raw;
}
