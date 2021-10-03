#include "next.h"
#include "../util/profile.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <tuple>
#include <vector>

Board nextBoard(const Board &board) {
  const auto &[input, width, height] = board;
  if (height == 0)
    return board;
  if (width == 0)
    return board;

  auto output = new Cell[width * height];

  int neighbours[3];
  int yAboveBase;
  int yBelowBase;
  int yBase;

  for (int i = 0; i < width * height; i++) {
    const int x = i % width;
    auto currentStateBool = input[i] == ALIVE ? 1 : 0;

    // Slide neighbours
    if (x == 0) {
      // Clear neighbour columns
      neighbours[0] = -1;
      neighbours[1] = -1;

      // Compute new Y levels
      const int y = i / width;
      yBase = y * width;
      yBelowBase = ((y - 1 + height) % height) * width;
      yAboveBase = ((y + 1) % height) * width;
    } else {
      neighbours[0] = neighbours[1];
      // Remove own value
      neighbours[1] = neighbours[2] - currentStateBool;
    }

    // Compute neighbours
    if (neighbours[0] == -1) {
      const auto previousX = (x - 1 + width) % width;

      neighbours[0] = (input[yBelowBase + previousX] ? 1 : 0) +
                      (input[yBase + previousX] ? 1 : 0) +
                      (input[yAboveBase + previousX] ? 1 : 0);
    }
    if (neighbours[1] == -1) {
      neighbours[1] = (input[yBelowBase + x] ? 1 : 0) +
                      (input[yAboveBase + x] ? 1 : 0);
    }
    const auto nextX = (x + 1) % width;
    neighbours[2] = (input[yBelowBase + nextX] ? 1 : 0) +
                    (input[yBase + nextX] ? 1 : 0) +
                    (input[yAboveBase + nextX] ? 1 : 0);

    // Compute new cell state
    const int totalNeighbours = neighbours[0] + neighbours[1] + neighbours[2];
    if (currentStateBool && (totalNeighbours < 2 || totalNeighbours > 3))
      output[i] = DEAD;
    else if (!currentStateBool && totalNeighbours == 3)
      output[i] = ALIVE;
    else
      output[i] = input[i];

    // Add self to neighbours count
    neighbours[1] += currentStateBool;
  }

  free(input);

  return {output, width, height};
}
