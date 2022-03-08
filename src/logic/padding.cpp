using uint = unsigned int;
#include "padding.h"

void assignPadding(Cell* cells, uint innerWidth, uint innerHeight) {
  const uint width = innerWidth + PADDING;
  const uint height = innerHeight + PADDING;

  for (uint i = 1; i <= width - 1; i++) {
    cells[INDEX(i)] = cells[INDEX(i + width * innerHeight)];
  }
  for (uint i = width * (height - 1) + 1; i <= width * height - 2; i++) {
    cells[INDEX(i)] = cells[INDEX(i - width * innerHeight)];
  }
  for (uint i = width; i <= width * (height - 1); i += width) {
    cells[INDEX(i)] = cells[INDEX(i + innerWidth)];
  }
  for (uint i = (width * 2) - 1; i <= width * (height - 1) - 1; i += width) {
    cells[INDEX(i)] = cells[INDEX(i - innerWidth)];
  }
  cells[INDEX(0)] = cells[INDEX(width * (height - 1) - 2)];
  cells[INDEX(width - 1)] = cells[INDEX(width * (height - 2) + 1)];
  cells[INDEX(width * (height - 1))] = cells[INDEX(width * 2 - 2)];
  cells[INDEX(width * height - 1)] = cells[INDEX(width + 1)];
}

void assignSkips(Cell* cells, uint innerWidth, uint innerHeight) {
  const uint width = innerWidth + PADDING;
  const uint height = innerHeight + PADDING;

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
  assignPadding(board.output, board.width, board.height);
  assignSkips(board.outSkip, board.width, board.height);
}
