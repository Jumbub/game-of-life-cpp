#include "next.h"
#include "../util/profile.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <tuple>
#include <vector>

void computeNeighbourColumns(char (&counts)[3], const Board &board, const int &x,
                       const int &y) {
  const auto &[input, width, height] = board;

  for (int ox = -1; ox <= 1; ox++) {
    counts[ox + 1] = 0;
    for (int oy = -1; oy <= 1; oy++) {
      // Do not count self
      if (oy == 0 && ox == 0)
        continue;

      // Real offset calculations
      const auto rox = (ox + x + width) % width;
      const auto roy = (oy + y + height) % height;

      if (input[roy * width + rox] == ALIVE)
        counts[ox + 1]++;
    }
  }
}

Board nextBoard(const Board &board) {
  const auto &[input, width, height] = board;
  if (height == 0)
    return board;
  if (width == 0)
    return board;

  auto output = new Cell[width * height];

  char neighbours[3] = {0, 0, 0};

  for (int i = 0; i < width * height; i++) {
    const int x = i % width;
    const int y = i / width;
    auto currentState = input[i];
    auto currentStateBool = input[i] == ALIVE ? 1 : 0;

    // Compute neighbour columns
    if (x == 0) {
      computeNeighbourColumns(neighbours, board, x, y);
    } else {
      // Shift neighbour columns down
      neighbours[0] = neighbours[1];
      neighbours[1] = neighbours[2];
      neighbours[2] = 0;

      // Remove self from neighbours
      neighbours[1] -= currentStateBool;

      // Compute next column of neighbours
      const int nextX = (x + 1) % width;
      for (int offsetY = -1; offsetY <= 1; offsetY++) {
        const int neighbourI = nextX + ((y + offsetY + height) % height) * width;
        neighbours[2] += input[neighbourI] == ALIVE ? 1 : 0;
      }
    }

    // Compute new cell state
    const int totalNeighbours = neighbours[0] + neighbours[1] + neighbours[2];
    if (currentState && (totalNeighbours < 2 || totalNeighbours > 3))
      output[i] = DEAD;
    else if (!currentState && totalNeighbours == 3)
      output[i] = ALIVE;
    else
      output[i] = currentState;

    // Add self to neighbours count
    neighbours[1] += currentStateBool;
  }

  free(input);

  return {output, width, height};
}
