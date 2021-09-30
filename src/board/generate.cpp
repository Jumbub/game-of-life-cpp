#include "generate.h"
#include "breeder.h"
#include <filesystem>
#include <random>
#include <stdexcept>
#include <memory>
#include <tuple>

Board randomBoard(int width, int height) {
  auto board = std::shared_ptr<bool[]>(new bool[width*height]);
  for (int i = 0; i < height*width; ++i)
    board[i] = rand() % 2;
  return {board, width, height};
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
  auto board = std::shared_ptr<bool[]>(new bool[width*height]);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const int i = y * width + x;
      if (y > height / 2) {
        if (x < width / 2)
          board[i] = rand() % 2;
        if (x >= width / 2)
          board[i] = (x / 8) % 2 != (y / 8) % 2;
      } else {
        const int breederMarginY = (height / 2 - BREEDER_HEIGHT) / 2;
        const int breederY = y - breederMarginY;
        if (breederY > 0 && breederY < BREEDER_HEIGHT && x < BREEDER_WIDTH) {
          board[i] = BREEDER[breederY][x];
        } else {
          board[i] = false;
        }
      }
    }
  }
  return {board, width, height};
}
