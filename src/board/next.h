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

inline uint64_t uint8s_to_uint64(Cell* ns) {
  return *reinterpret_cast<uint64_t*>(ns);
}

constexpr uint8_t LOOKUP[20] = {
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
};

const uint64_t SKIP_ALL =
    (1 << 0) + (1 << 8) + (1 << 16) + (1 << 24) + (1l << 32) + (1l << 40) + (1l << 48) + (1l << 56);

void whileSkip8s(uint& i, uint8_t* skips) {
  while (uint8s_to_uint64(&skips[i]) == SKIP_ALL) {
    i += 8;
  }
}

void whileSkip1s(uint& i, const uint8_t* skips) {
  while (skips[i]) {
    i++;
  }
}

uint isAlive(const uint& i, const Cell* input, const uint& realWidth) {
  const Cell* top = &input[i - realWidth - 1];
  const Cell* middle = &input[i - 1];
  const Cell* bottom = &input[i + realWidth - 1];

  const auto a = top[0] + middle[0] + bottom[0];
  const auto b = top[1] + middle[1] * 9 + bottom[1];
  const auto c = top[2] + middle[2] + bottom[2];

  return LOOKUP[a + b + c];
}

inline void revokeSkipForNeighbours(const uint& i, Cell* skips, const uint& realWidth) {
  skips[i - realWidth - 1] = false;
  skips[i - realWidth] = false;
  skips[i - realWidth + 1] = false;

  skips[i - 1] = false;
  skips[i] = false;
  skips[i + 1] = false;

  skips[i + realWidth - 1] = false;
  skips[i + realWidth] = false;
  skips[i + realWidth + 1] = false;
}

void nextBoardSection(
    const uint startY,
    const uint endY,
    const uint realWidth,
    Cell* input,
    Cell* output,
    uint8_t* inSkip,
    uint8_t* outSkip) {
  uint i = startY * realWidth + 1;
  const uint stop = endY * realWidth - 1;

  while (i < stop) {
    whileSkip8s(i, inSkip);
    whileSkip1s(i, inSkip);

    output[i] = isAlive(i, input, realWidth);

    if (input[i] != output[i]) {
      revokeSkipForNeighbours(i, outSkip, realWidth);
    }

    i++;
  }
}

void nextBoard(Board& board, const uint& threadCount) {
  board.setOutputToInput();

  const auto& height = board.height;

  const auto totalThreads = std::min(threadCount, (uint)height);
  const auto threadLines = height / totalThreads;
  const auto threadLinesRemaining = height % totalThreads;

  const auto& realWidth = board.width + 2;
  const auto size = realWidth * (height + 2);

  std::memset(board.outSkip, true, sizeof(Cell) * size);
  board.inSkip[size - realWidth - 1] = false;  // Ensure the last cell is never skipped to minimise branches

  std::vector<std::thread> threads;
  for (uint t = 0; t < totalThreads; t++) {
    // Compute start and end indexes for threads
    const auto startY = t * threadLines + 1;
    auto endY = (t + 1) * threadLines + 1;

    // In the case of an uneven divide, the last thread gets the left-overs
    if (t == totalThreads - 1)
      endY += threadLinesRemaining;

    threads.push_back(std::thread([startY, endY, &board, &realWidth]() {
      nextBoardSection(startY, endY, realWidth, board.input, board.output, board.inSkip, board.outSkip);
    }));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  assignBoardPadding(board);
}
