#include "next.h"
#include "../util/profile.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <tuple>
#include <vector>

const NeighbourPositions getNeighbourPositions(const int px, const int py,
                                               const int width,
                                               const int height) {
  NeighbourPositions positions = {};
  int i = 0;
  for (int y = -1; y <= 1; y++)
    for (int x = -1; x <= 1; x++) {
      if (y == 0 && x == 0)
        continue;

      auto xx = (px + x) % width;
      if (xx < 0)
        xx = xx + width;
      auto yy = (py + y) % height;
      if (yy < 0)
        yy = yy + height;

      positions[i++] = yy * width + xx;
    }
  return positions;
}

Board nextBoard(Board board) {
  const auto &[input, width, height] = board;
  if (height == 0)
    return board;
  if (width == 0)
    return board;

  auto output = std::shared_ptr<bool[]>(new bool[width*height]);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      const int i = y * width + x;

      auto alive = input[i];

      auto neighboursPositions = getNeighbourPositions(x, y, width, height);
      int neighboursCount = 0;
      for (int n = 0; n < 8; n++) {
        if (input[neighboursPositions[n]])
          neighboursCount++;

        // Optimisation (exit early if we know it's a dead cell)
        if (neighboursCount > 3) {
          break;
        }
      }

      if (alive && (neighboursCount < 2 || neighboursCount > 3))
        output[i] = false;
      else if (!alive && neighboursCount == 3)
        output[i] = true;
      else
        output[i] = alive;
    }
  }

  return {output, width, height};
}
