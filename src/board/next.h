#pragma once

#include <algorithm>
#include <array>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>
#include "board.h"
#include "padding.h"
#include "threads.h"

inline uint64_t uint8_to_uint64(Cell* ns) {
  return *reinterpret_cast<uint64_t*>(ns);
}

void nextBoard(Board& board, [[maybe_unused]] const uint& threadCount) {
  board.setOutputToInput();

  const auto& width = board.width;
  const auto& height = board.height;
  const auto& input = board.input;
  const auto& output = board.output;

  const auto rw = width + 2;
  const auto rh = height + 2;
  for (uint i = rw + 1; i < (rw * rh) - rw - 1; i++) {
    const auto alive = input[i][0];
    const auto neighbours = input[i][1];

    const auto next = neighbours == 3 || (alive && neighbours == 2);
    output[i][0] = next;
  }

  assignBoardPadding(board, 0);
  assignBoardNeighbours(board);
  assignBoardPadding(board, 1);
}
