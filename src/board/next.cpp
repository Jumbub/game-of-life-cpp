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

  auto output = new Cell[width*height];

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      const int i = y * width + x;

      auto state = input[i];

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

      if (state && (neighboursCount < 2 || neighboursCount > 3))
        output[i] = DEAD;
      else if (!state && neighboursCount == 3)
        output[i] = ALIVE;
      else
        output[i] = state;
    }
  }

  free(input);

  return {output, width, height};
}
