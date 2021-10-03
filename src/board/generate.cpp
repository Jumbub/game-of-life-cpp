#include "generate.h"
#include "breeder.h"
#include <filesystem>
#include <memory>
#include <random>
#include <stdexcept>
#include <tuple>

Board randomBoard(int width, int height) {
  auto board = new Cell[width * height];
  for (int i = 0; i < height * width; ++i)
    board[i] = rand() % 2 ? ALIVE : DEAD;
  return {board, width, height};
}

/**
 * Generates a board with 4 regions:
 * - top-left:     seeded random cells
 * - top-right:    chess grid of 8x8 alive/dead cells
 * - bottom:       breeder
 */
Board benchmarkBoard(int width, int height) {
  srand(0);
  auto board = new Cell[width * height];
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const int i = y * width + x;
      if (y > height / 2) {
        if (x < width / 2)
          board[i] = rand() % 2 ? ALIVE : DEAD;
        if (x >= width / 2)
          board[i] = (x / 8) % 2 != (y / 8) % 2 ? ALIVE : DEAD;
      } else {
        const int breederMarginY = (height / 2 - BREEDER_HEIGHT) / 2;
        const int breederY = y - breederMarginY;
        if (breederY > 0 && breederY < BREEDER_HEIGHT && x < BREEDER_WIDTH) {
          board[i] = BREEDER[breederY][x] ? ALIVE : DEAD;
        } else {
          board[i] = DEAD;
        }
      }
    }
  }
  return {board, width, height};
}
