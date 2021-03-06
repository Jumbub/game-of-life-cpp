using uint = unsigned int;
#include "padding.h"

void assignPadding(Cell* cells, uint innerWidth, uint innerHeight) {
  const uint width = innerWidth + PADDING;
  const uint height = innerHeight + PADDING;

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

void assignSkips(Cell* cells, uint innerWidth, uint innerHeight) {
  const uint width = innerWidth + PADDING;
  const uint height = innerHeight + PADDING;

  for (uint i = 1; i <= width - 1; i++) {
    cells[(i + width * innerHeight) / SKIPS_PER_BYTE] = NO_SKIP_BYTE;
  }
  for (uint i = width * (height - 1) + 1; i <= width * height - 2; i++) {
    cells[(i - width * innerHeight) / SKIPS_PER_BYTE] = NO_SKIP_BYTE;
  }
  for (uint i = width; i <= width * (height - 1); i += width) {
    cells[(i + innerWidth) / SKIPS_PER_BYTE] = NO_SKIP_BYTE;
  }
  for (uint i = (width * 2) - 1; i <= width * (height - 1) - 1; i += width) {
    cells[(i - innerWidth) / SKIPS_PER_BYTE] = NO_SKIP_BYTE;
  }
  cells[(0) / SKIPS_PER_BYTE] = NO_SKIP_BYTE;
  cells[(width - 1) / SKIPS_PER_BYTE] = NO_SKIP_BYTE;
  cells[(width * (height - 1)) / SKIPS_PER_BYTE] = NO_SKIP_BYTE;
  cells[(width * height - 1) / SKIPS_PER_BYTE] = NO_SKIP_BYTE;
}

void assignBoardPadding(Board& board) {
  assignPadding(board.output, board.width, board.height);
  assignSkips(board.outSkip, board.width, board.height);
}
