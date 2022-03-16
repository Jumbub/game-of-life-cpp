using uint = unsigned int;
#include "padding.h"

void assignPadding(Board& board) {
  auto& cells = board.output;
  const auto innerWidth = board.width;
  const auto innerHeight = board.height;
  const auto width = board.paddedWidth;
  const auto height = board.paddedHeight;

  for (uint i = 1; i <= width - 1; i++) {
    cells[i] = cells[i + width * innerHeight];
  }
  for (uint i = width * (height - 1) + 1; i <= width * height - 2; i++) {
    cells[i] = cells[i - width * innerHeight];
  }
  for (uint i = width; i <= width * (height - 1); i += width) {
    cells[i] = cells[i + innerWidth];
  }
  for (uint i = (width * 2) - 1; i <= width * (height - 1) - 1; i += width) {
    cells[i] = cells[i - innerWidth];
  }
  cells[0] = cells[width * (height - 1) - 2];
  cells[width - 1] = cells[width * (height - 2) + 1];
  cells[width * (height - 1)] = cells[width * 2 - 2];
  cells[width * height - 1] = cells[width + 1];
}

void assignJobs(Board& board) {
  auto& cells = board.nextJobs;
  const auto innerWidth = board.width;
  const auto innerHeight = board.height;
  const auto width = board.paddedWidth;
  const auto height = board.paddedHeight;

  for (uint i = 1; i <= width - 1; i++) {
    cells[i + width * innerHeight] = cells[i + width * innerHeight] & cells[i];
  }
  for (uint i = width * (height - 1) + 1; i <= width * height - 2; i++) {
    cells[i - width * innerHeight] = cells[i - width * innerHeight] & cells[i];
  }
  for (uint i = width; i <= width * (height - 1); i += width) {
    cells[i + innerWidth] = cells[i + innerWidth] & cells[i];
  }
  for (uint i = (width * 2) - 1; i <= width * (height - 1) - 1; i += width) {
    cells[i - innerWidth] = cells[i - innerWidth] & cells[i];
  }
  cells[0] = cells[0] & cells[width * (height - 1) - 2];
  cells[width - 1] = cells[width - 1] & cells[width * (height - 2) + 1];
  cells[width * (height - 1)] = cells[width * (height - 1)] & cells[width * 2 - 2];
  cells[width * height - 1] = cells[width * height - 1] & cells[width + 1];
}

void assignBoardPadding(Board& board) {
  assignPadding(board);
  assignJobs(board);
}
