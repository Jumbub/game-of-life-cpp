#include "generate.h"
#include <random>

Board randomBoard(int width, int height) {
  Board board(height, std::vector<bool>(width));
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      board[y][x] = rand() % 2;
    }
  }
  return board;
}

/**
 * Generates a board with 4 regions:
 * - top-left:     seeded random cells
 * - top-right:    chess grid of 8x8 alive/dead cells
 * - bottom-left:  spaceships, oscillators
 * - bottom-right: still lifes
 */
Board benchmarkBoard(int width, int height) {
  srand(0);
  Board board(height, std::vector<bool>(width));
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (x < width / 2 && y < height / 2)
        board[y][x] = rand() % 2;
      if (x >= width / 2 && y < height / 2)
        board[y][x] = (x / 8) % 2 != (y / 8) % 2;
    }
  }
  return board;
}
