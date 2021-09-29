#include "random.h"
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
