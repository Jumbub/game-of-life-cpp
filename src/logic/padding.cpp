using uint = unsigned int;
#include "padding.h"

void assignPadding(Board& board) {
  auto cells = board.output;
  const auto innerWidth = board.width;
  const auto innerHeight = board.height;
  const auto width = board.paddedWidth;
  const auto height = board.paddedHeight;

  for (uint i = 1; i <= width - 1; i++) {
    cells.set(i, cells.test(i + width * innerHeight));
  }
  for (uint i = width * (height - 1) + 1; i <= width * height - 2; i++) {
    cells.set(i, cells.test(i - width * innerHeight));
  }
  for (uint i = width; i <= width * (height - 1); i += width) {
    cells.set(i, cells.test(i + innerWidth));
  }
  for (uint i = (width * 2) - 1; i <= width * (height - 1) - 1; i += width) {
    cells.set(i, cells.test(i - innerWidth));
  }
  cells.set(0, cells.test(width * (height - 1) - 2));
  cells.set(width - 1, cells.test(width * (height - 2) + 1));
  cells.set(width * (height - 1), cells.test(width * 2 - 2));
  cells.set(width * height - 1, cells.test(width + 1));
}

void assignJobs(Board& board) {
  auto cells = board.output;
  const auto innerWidth = board.width;
  const auto innerHeight = board.height;
  const auto width = board.paddedWidth;
  const auto height = board.paddedHeight;

  for (uint i = 1; i <= width - 1; i++) {
    cells.set(i + width * innerHeight, cells.test(i + width * innerHeight) & cells.test(i));
  }
  for (uint i = width * (height - 1) + 1; i <= width * height - 2; i++) {
    cells.set(i - width * innerHeight, cells.test(i - width * innerHeight) & cells.test(i));
  }
  for (uint i = width; i <= width * (height - 1); i += width) {
    cells.set(i + innerWidth, cells.test(i + innerWidth) & cells.test(i));
  }
  for (uint i = (width * 2) - 1; i <= width * (height - 1) - 1; i += width) {
    cells.set(i - innerWidth, cells.test(i - innerWidth) & cells.test(i));
  }
  cells.set(0, cells.test(0) & cells.test(width * (height - 1) - 2));
  cells.set(width - 1, cells.test(width - 1) & cells.test(width * (height - 2) + 1));
  cells.set(width * (height - 1), cells.test(width * (height - 1)) & cells.test(width * 2 - 2));
  cells.set(width * height - 1, cells.test(width * height - 1) & cells.test(width + 1));
}

void assignBoardPadding(Board& board) {
  assignPadding(board);
  assignJobs(board);
}
