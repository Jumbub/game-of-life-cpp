#include <algorithm>
#include <cstring>
#include "../common/lock.h"
#include "board.h"

void Board::setOutputToInput() {
  input.swap(output);
  jobs.swap(nextJobs);
}

void Board::setSize(const uint& width, const uint& height) {
  this->width = width;
  this->height = height;

  this->paddedWidth = width + PADDING * 2;
  this->paddedHeight = height + PADDING * 2;
  this->paddedSize = paddedWidth * paddedHeight;

  input.resize(paddedSize, DEAD);
  input.reset();

  output.resize(paddedSize, DEAD);
  output.reset();

  jobs.resize(paddedSize, COMPUTE);
  jobs.set();

  nextJobs.resize(paddedSize, COMPUTE);
  nextJobs.set();
}

void Board::setJobs() {
  jobs.set();
  nextJobs.set();
}

Board::Board(const uint& width, const uint& height) {
  setSize(width, height);
}
