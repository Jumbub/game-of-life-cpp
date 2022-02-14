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

constexpr uint8_t LOOKUP[20] = {
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
};

void nextBoardSection(const uint startY, const uint endY, const uint width, Cell* input, Cell* output) {
  const auto realWidth = width + 2;
  const auto realStart = startY + 1;
  const auto realEnd = endY + 1;

  const uint start = realStart * realWidth + 1;

  Cell* top = &input[start - realWidth - 1];
  Cell* middle = &input[start - 1];
  Cell* bottom = &input[start + realWidth - 1];

  const uint stop = realEnd * realWidth - 1;
  for (uint i = start; i < stop; i++) {
    if (uint8_to_uint64(top) + uint8_to_uint64(middle) + uint8_to_uint64(bottom) == 0) {
      output[i] = DEAD;
      output[i + 1] = DEAD;
      output[i + 2] = DEAD;
      output[i + 3] = DEAD;
      output[i + 4] = DEAD;
      output[i + 5] = DEAD;

      top += 6;
      middle += 6;
      bottom += 6;

      i += 5;
    } else {
      const auto a = top[0] + middle[0] + bottom[0];
      const auto b = top[1] + middle[1] * 9 + bottom[1];
      const auto c = top[2] + middle[2] + bottom[2];

      output[i] = LOOKUP[a + b + c];

      top++;
      middle++;
      bottom++;
    }
  }
}

void nextBoard(Board& board, const uint& threadCount) {
  board.setOutputToInput();

  const auto& width = board.width;
  const auto& height = board.height;
  const auto& input = board.input;
  const auto& output = board.output;

  const auto totalThreads = std::min(threadCount, (uint)height);
  const auto threadLines = height / totalThreads;
  const auto threadLinesRemaining = height % totalThreads;

  std::vector<std::thread> threads;
  for (uint t = 0; t < totalThreads; t++) {
    // Compute start and end indexes for threads
    const auto startY = t * threadLines;
    auto endY = (t + 1) * threadLines;

    // In the case of an uneven divide, the last thread gets the left-overs
    if (t == totalThreads - 1)
      endY += threadLinesRemaining;

    threads.push_back(
        std::thread([startY, endY, width, input, output]() { nextBoardSection(startY, endY, width, input, output); }));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  assignBoardPadding(board.output, board.width, board.height);
}
