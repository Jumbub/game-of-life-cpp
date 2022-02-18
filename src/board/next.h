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

void nextBoardSection(
    const uint startY,
    const uint endY,
    const uint width,
    Cell* input,
    Cell* output,
    uint8_t* inSkip,
    uint8_t* outSkip) {
  const auto realWidth = width + 2;
  const auto realStart = startY + 1;
  const auto realEnd = endY + 1;

  const uint start = realStart * realWidth + 1;

  Cell* top = &input[start - realWidth - 1];
  Cell* middle = &input[start - 1];
  Cell* bottom = &input[start + realWidth - 1];

  const uint stop = realEnd * realWidth - 1;
  for (uint i = start; i < stop; i++) {
    while (uint8s_to_uint64(&inSkip[i]) == SKIP_ALL) {
      i += 8;
    }
    while (inSkip[i]) {
      i++;
    }

    top = &input[i - realWidth - 1];
    middle = &input[i - 1];
    bottom = &input[i + realWidth - 1];

    const auto a = top[0] + middle[0] + bottom[0];
    const auto b = top[1] + middle[1] * 9 + bottom[1];
    const auto c = top[2] + middle[2] + bottom[2];

    const auto ns = a + b + c;
    output[i] = LOOKUP[ns];

    if (input[i] != output[i]) {
      outSkip[i - realWidth - 1] = false;
      outSkip[i - realWidth] = false;
      outSkip[i - realWidth + 1] = false;

      outSkip[i - 1] = false;
      outSkip[i] = false;
      outSkip[i + 1] = false;

      outSkip[i + realWidth - 1] = false;
      outSkip[i + realWidth] = false;
      outSkip[i + realWidth + 1] = false;
    }
  }
}

void nextBoard(Board& board, const uint& threadCount) {
  board.setOutputToInput();

  const auto& width = board.width;
  const auto& height = board.height;
  const auto& input = board.input;
  const auto& output = board.output;
  const auto& inSkip = board.inSkip;
  const auto& outSkip = board.outSkip;

  const auto totalThreads = std::min(threadCount, (uint)height);
  const auto threadLines = height / totalThreads;
  const auto threadLinesRemaining = height % totalThreads;

  const auto size = (width + 2) * (height + 2);

  std::memset(board.outSkip, true, sizeof(Cell) * size);
  board.inSkip[size - (width + 2) - 1] = false;  // Ensure the last cell is never skipped to minimise branches

  std::vector<std::thread> threads;
  for (uint t = 0; t < totalThreads; t++) {
    // Compute start and end indexes for threads
    const auto startY = t * threadLines;
    auto endY = (t + 1) * threadLines;

    // In the case of an uneven divide, the last thread gets the left-overs
    if (t == totalThreads - 1)
      endY += threadLinesRemaining;

    threads.push_back(std::thread([startY, endY, width, input, output, inSkip, outSkip]() {
      nextBoardSection(startY, endY, width, input, output, inSkip, outSkip);
    }));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  assignBoardPadding(board);
}
