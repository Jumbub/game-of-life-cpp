#pragma once

#include <algorithm>
#include <array>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>
#include "board.h"

void assignBoardPadding(Board& board, int index) {
  const auto innerWidth = board.width;
  const auto innerHeight = board.height;
  const auto cells = board.output;
  const auto width = board.width + 2;
  const auto height = board.height + 2;

  for (uint i = 1; i <= width - 1; i++) {
    cells[i][index] = cells[i + width * innerHeight][index];
  }

  for (uint i = width * (height - 1) + 1; i <= width * height - 2; i++) {
    cells[i][index] = cells[i - width * innerHeight][index];
  }

  for (uint i = width; i <= width * (height - 1); i += width) {
    cells[i][index] = cells[i + innerWidth][index];
  }

  for (uint i = (width * 2) - 1; i <= width * (height - 1) - 1; i += width) {
    cells[i][index] = cells[i - innerWidth][index];
  }

  cells[0][index] = cells[width * (height - 1) - 2][index];
  cells[width - 1][index] = cells[width * (height - 2) + 1][index];
  cells[width * (height - 1)][index] = cells[width * 2 - 2][index];
  cells[width * height - 1][index] = cells[width + 1][index];
}

inline uint8_t assignCellNeighbours(Board& board, uint i) {
  const auto cells = board.output;
  const auto width = board.width + 2;

  return cells[i - width - 1][0] + cells[i - width][0] + cells[i - width + 1][0] +

         cells[i - 1][0] + cells[i + 1][0]

         + cells[i + width - 1][0] + cells[i + width][0] + cells[i + width + 1][0];
}

void assignBoardNeighbours(Board& board) {
  const auto cells = board.output;
  const auto width = board.width + 2;
  const auto height = board.height + 2;

  for (uint i = width + 1; i < (width * height) - width; i++)
    cells[i][1] = assignCellNeighbours(board, i);
}

void assignBoardEdgeNeighbours(Board& board) {
  const auto cells = board.output;
  const auto width = board.width + 2;
  const auto height = board.height + 2;

  for (uint i = width + 1; i < (width * height) - width; i++) {
    if (i % width == 1 || i % width == width - 2 || i % height == 1 || i % height == height - 2) {
      cells[i][1] = assignCellNeighbours(board, i);
    }
  }
}

void assignBorders(Board& board) {
  assignBoardPadding(board, 0);
  assignBoardEdgeNeighbours(board);
  assignBoardPadding(board, 1);
}
