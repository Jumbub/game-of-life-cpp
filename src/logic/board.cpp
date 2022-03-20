#include <algorithm>
#include <cstring>
#include "../common/lock.h"
#include "board.h"

void Board::setOutputToInput() {
  std::swap(input, output);
  std::swap(inSkip, outSkip);
}

void Board::setSize(const uint& width, const uint& height) {
  this->width = width;
  this->height = height;

  this->rawWidth = width + PADDING;
  this->rawHeight = height + PADDING;
  this->rawSize = rawWidth * rawHeight;

  if (raw != nullptr)
    delete[] raw;

  raw = new Cell[rawSize * 4];
  input = raw;
  output = input + rawSize;
  inSkip = output + rawSize;
  outSkip = inSkip + rawSize;

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
