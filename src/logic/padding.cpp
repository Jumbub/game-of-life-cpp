using uint = unsigned int;
#include <iostream>
#include "padding.h"

void assignPadding(Board& board) {
  auto& cells = board.input;
  const auto width = board.paddedWidth;
  const auto height = board.paddedHeight;

  /* std::cout << "assignPadding" << width << " " << height << std::endl; */

  /* for (uint i = 0; i < width * height; i++) */
  /*   std::cout << cells[i]; */
  /* std::cout << std::endl; */

  for (uint i = 1; i <= width - 1; i++) {
    cells.set(i, cells.test(i + width * (height - 2)));
  }
  for (uint i = width * (height - 1) + 1; i <= width * height - 2; i++) {
    cells.set(i, cells.test(i - width * (height - 2)));
  }
  for (uint i = width; i <= width * (height - 1); i += width) {
    cells.set(i, cells.test(i + (width - 2)));
  }
  for (uint i = (width * 2) - 1; i <= width * (height - 1) - 1; i += width) {
    cells.set(i, cells.test(i - (width - 2)));
  }
  cells.set(0, cells.test(width * (height - 1) - 2));
  cells.set(width - 1, cells.test(width * (height - 2) + 1));
  cells.set(width * (height - 1), cells.test(width * 2 - 2));
  cells.set(width * height - 1, cells.test(width + 1));

  /* for (uint i = 0; i < width * height; i++) */
  /*   std::cout << cells[i]; */
  /* std::cout << std::endl; */
}

void assignJobs(Board& board) {
  auto& cells = board.jobs;
  const auto width = board.paddedWidth;
  const auto height = board.paddedHeight;

  for (uint i = 1; i <= width - 1; i++) {
    cells.set(i + width * (height - 2), cells.test(i + width * (height - 2)) && cells.test(i));
  }
  for (uint i = width * (height - 1) + 1; i <= width * height - 2; i++) {
    cells.set(i - width * (height - 2), cells.test(i - width * (height - 2)) && cells.test(i));
  }
  for (uint i = width; i <= width * (height - 1); i += width) {
    cells.set(i + (width - 2), cells.test(i + (width - 2)) && cells.test(i));
  }
  for (uint i = (width * 2) - 1; i <= width * (height - 1) - 1; i += width) {
    cells.set(i - (width - 2), cells.test(i - (width - 2)) && cells.test(i));
  }
  cells.set(0, cells.test(0) && cells.test(width * (height - 1) - 2));
  cells.set(width - 1, cells.test(width - 1) && cells.test(width * (height - 2) + 1));
  cells.set(width * (height - 1), cells.test(width * (height - 1)) && cells.test(width * 2 - 2));
  cells.set(width * height - 1, cells.test(width * height - 1) && cells.test(width + 1));
}

void assignBoardPadding(Board& board) {
  assignPadding(board);
  assignJobs(board);
}
