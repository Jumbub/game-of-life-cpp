#include "next.h"
#include "../util/profile.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <tuple>
#include <vector>

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

      int neighboursCount = 0;
      for (int y2 = -1; y2 <= 1; y2++)
        for (int x2 = -1; x2 <= 1; x2++) {
          if (y2 == 0 && x2 == 0)
            continue;

          auto xx = (x2 + x) % width;
          if (xx < 0)
            xx = xx + width;
          auto yy = (y2 + y) % height;
          if (yy < 0)
            yy = yy + height;

          if (input[yy * width + xx])
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
