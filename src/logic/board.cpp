#include <algorithm>
#include <cstring>
#include "../common/lock.h"
#include "board.h"

void Board::setOutputToInput() {
  input.swap(output);
  skips.swap(nextSkips);
}

void Board::setSize(const uint& width, const uint& height) {
  this->width = width;
  this->height = height;

  this->paddedWidth = width + PADDING * 2;
  this->paddedHeight = height + PADDING * 2;
  this->paddedSize = paddedWidth * paddedHeight;

  input.resize(paddedSize, false);
  input.reset();

  output.resize(paddedSize, false);
  output.reset();

  skips.resize(paddedSize, false);
  skips.reset();

  nextSkips.resize(paddedSize, false);
  nextSkips.reset();
}

void Board::clearSkips() {
  skips.reset();
  nextSkips.reset();
}

Board::Board(const uint& width, const uint& height) {
  setSize(width, height);
}
