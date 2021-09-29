#include "generate.h"
#include "breeder.h"
#include <filesystem>
#include <random>
#include <stdexcept>

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
 * - bottom:       breeder
 */
Board benchmarkBoard(int width, int height) {
  if (height < BREEDER_HEIGHT * 2)
    throw std::underflow_error(
        "Did not meet minimum height required for the benchmark board");

  srand(0);
  Board board(height, std::vector<bool>(width));
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (y > height / 2) {
        if (x < width / 2)
          board[y][x] = rand() % 2;
        if (x >= width / 2)
          board[y][x] = (x / 8) % 2 != (y / 8) % 2;
      } else {
        const int breederMarginY = (height / 2 - BREEDER_HEIGHT) / 2;
        const int breederY = y - breederMarginY;
        if (breederY > 0 && breederY < BREEDER_HEIGHT && x < BREEDER_WIDTH) {
          board[y][x] = BREEDER[breederY][x];
        } else {
          board[y][x] = false;
        }
      }
    }
  }
  return board;
}
