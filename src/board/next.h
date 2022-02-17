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

/*
void none() {
  const uint realWidth = width + 2;
  const uint realWidthX2 = realWidth * 2;

  const uint startI = (startY + 1) * realWidth;
  const uint lines = endY - startY;

  if (lines % 2 == 1)
    throw std::runtime_error("ohno");

  const auto yLimit = startI + realWidth * lines;
  for (auto y = startI; y < yLimit; y += realWidth * 2) {
    Cell* top = input + y - realWidth;
    Cell* middle = input + y;
    Cell* middle2 = input + y + realWidth;
    Cell* bottom = input + y + realWidthX2;

    const auto xLimit = y + realWidth - 1;
    for (uint x = y + 1; x < xLimit; x++) {
      const auto i = y + x;

      const auto a = top[0] + middle[0] + middle2[0];
      const auto b = top[1] + middle[1] * 9 + middle2[1];
      const auto c = top[2] + middle[2] + middle2[2];

      const auto A = middle[0] + middle2[0] + bottom[0];
      const auto B = middle[1] + middle2[1] * 9 + bottom[1];
      const auto C = middle[2] + middle2[2] + bottom[2];

      output[i] = LOOKUP[a + b + c];
      output[i + realWidth] = LOOKUP[A + B + C];

      top++;
      middle++;
      middle2++;
      bottom++;
    }
  }
}
*/

void nextBoardSection(const uint startY, const uint endY, const uint width, Cell* input, Cell* output) {
  const auto realWidth = width + 2;

  for (uint y = startY + 1; y < endY + 1; y += 2) {
    Cell* top = &input[(y - 1) * realWidth];
    Cell* middle = top + realWidth;
    Cell* middle2 = middle + realWidth;
    Cell* bottom = middle2 + realWidth;

    const auto stopI = (y + 1) * realWidth - 1;
    for (uint i = y * realWidth + 1; i < stopI; i++) {
      const auto i2 = i + realWidth;
      if (uint8_to_uint64(top) + uint8_to_uint64(middle) + uint8_to_uint64(middle2) + uint8_to_uint64(bottom) == 0) {
        output[i] = DEAD;
        output[i + 1] = DEAD;
        output[i + 2] = DEAD;
        output[i + 3] = DEAD;
        output[i + 4] = DEAD;
        output[i + 5] = DEAD;

        output[i2] = DEAD;
        output[i2 + 1] = DEAD;
        output[i2 + 2] = DEAD;
        output[i2 + 3] = DEAD;
        output[i2 + 4] = DEAD;
        output[i2 + 5] = DEAD;

        top += 6;
        middle += 6;
        middle2 += 6;
        bottom += 6;

        i += 5;
      } else {
        const auto topTotal = top[0] + top[1] + top[2];
        const auto middlePartial = middle[0] + middle[2];
        const auto middleMiddle = middle[1];
        const auto middle2Partial = middle2[0] + middle2[2];
        const auto middle2Middle = middle2[1];
        const auto bottomTotal = bottom[0] + bottom[1] + bottom[2];

        output[i] = LOOKUP[topTotal + (middlePartial + middleMiddle * 9) + (middle2Partial + middle2Middle)];
        output[i2] = LOOKUP[(middlePartial + middleMiddle) + (middle2Partial + middle2Middle * 9) + bottomTotal];

        top++;
        middle++;
        middle2++;
        bottom++;
      }
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
