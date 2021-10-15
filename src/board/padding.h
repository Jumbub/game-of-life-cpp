#pragma once

#include <algorithm>
#include <array>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>
#include "board.h"

void padding(Cell* cells, uint innerWidth, uint innerHeight) {
  const uint width = innerWidth + 2;
  const uint height = innerHeight + 2;

  for (uint i = 1; i <= width - 1; i++) {
    cells[i] = cells[i + width*innerHeight];
  }
  for (uint i = width*(height-1)+1; i <= width*height-2; i++) {
    cells[i] = cells[i - width*innerHeight];
  }
  for (uint i = width; i <= width*(height-1); i+=width) {
    cells[i] = cells[i + innerWidth];
  }
  for (uint i = (width*2)-1; i <= width*(height-1)-1; i+=width) {
    cells[i] = cells[i - innerWidth];
  }
  cells[0] = cells[width*(height-1)-2];
  cells[width-1] = cells[width*(height-2)+1];
  cells[width*(height-1)] = cells[width*2-2];
  cells[width*height-1] = cells[width+1];
}
