#include "next.h"
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

      positions[i++] = {xx, yy};
    }
  return positions;
}

Board nextBoard(Board input) {
  const int height = input.size();
  if (height == 0)
    return input;
  const int width = input[0].size();
  if (width == 0)
    return input;

  Board output(height, std::vector<bool>(width));

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto alive = input[y][x];
      auto neighbourKeys = getNeighbourPositions(x, y, width, height);

      std::array<bool, 8> neighbourValues = {};
      std::transform(neighbourKeys.begin(), neighbourKeys.end(),
                     neighbourValues.begin(), [&input](Position pos) -> bool {
                       const auto &[x, y] = pos;
                       return input[y][x];
                     });

      auto neighboursAlive =
          std::count(neighbourValues.begin(), neighbourValues.end(), true);

      if (alive && (neighboursAlive < 2 || neighboursAlive > 3))
        output[y][x] = false;
      else if (!alive && neighboursAlive == 3)
        output[y][x] = true;
      else
        output[y][x] = alive;
    }
  }

  return output;
}
