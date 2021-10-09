#pragma once

#include "board.h"
#include "breeder.h"
#include "generate.h"

/**
 * Generates a board with 4 regions:
 * - top-left:     seeded random cells
 * - top-right:    chess grid of 8x8 alive/dead cells
 * - bottom:       breeder
 */
void benchmarkBoard(BoardMeta& board, unsigned int width, unsigned int height) {
  srand(0);
  board.resize(width, height);
  for (unsigned int y = 0; y < height; ++y) {
    for (unsigned int x = 0; x < width; ++x) {
      const unsigned int i = y * width + x;
      if (y > height / 2) {
        if (x < width / 2)
          board.input[i] = rand() % 2 ? ALIVE : DEAD;
        if (x >= width / 2)
          board.input[i] = (x / 8) % 2 != (y / 8) % 2 ? ALIVE : DEAD;
      } else {
        const unsigned int breederMarginY = (height / 2 - BREEDER_HEIGHT) / 2;
        const unsigned int breederY = y - breederMarginY;
        if (breederY > 0 && breederY < BREEDER_HEIGHT && x < BREEDER_WIDTH) {
          board.input[i] = BREEDER[breederY][x] ? ALIVE : DEAD;
        } else {
          board.input[i] = DEAD;
        }
      }
    }
  }
}
