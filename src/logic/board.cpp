#include <algorithm>
#include <cstring>
#include "../common/lock.h"
#include "board.h"

void Board::setOutputToInput() {
  std::swap(input, output);
  std::swap(jobs, nextJobs);
}

void Board::setSize(const uint& width, const uint& height) {
  this->width = width;
  this->height = height;

  this->paddedWidth = width + PADDING * 2;
  this->paddedHeight = height + PADDING * 2;
  this->paddedSize = paddedWidth * paddedHeight;

  input.reset();
  output.reset();
  jobs.set();
  nextJobs.set();
}

void Board::setJobs() {
  jobs.set();
  nextJobs.set();
}

Board::Board(const uint& width, const uint& height) {
  setSize(width, height);
}
