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

void nextBoardSection(
    const uint startY,
    const uint endY,
    const uint width,
    const Cell* input,
    Cell* output) {
  Cell neighbours[3] = {0, 0, 0};
  uint nextYBase = 0;
  uint middleYBase = 0;
  uint lastYBase = 0;

  const Cell* neighboursBelow = nullptr;
  const Cell* neighboursMiddle = nullptr;
  const Cell* neighboursAbove = nullptr;

  const auto realWidth = width + 2;
  for (uint y = startY + 1; y < endY + 1; y++) {
    const auto currentStateBool = input[y + 1];

    // Compute new Y levels
    lastYBase = ((y - 1) * realWidth);
    middleYBase = y * realWidth;
    nextYBase = ((y + 1) * realWidth);

    neighboursBelow = &input[lastYBase];
    neighboursMiddle = &input[middleYBase];
    neighboursAbove = &input[nextYBase];

    // Left neighbours
    neighbours[0] =
        neighboursBelow[0] + neighboursMiddle[0] + neighboursAbove[0];

    // Middle neighbours
    neighbours[1] = neighboursBelow[1] + neighboursAbove[1] + currentStateBool;

    for (uint x = 1; x < width + 1; x++) {
      const auto i = y * realWidth + x;
      const auto currentStateBool = input[i];

      if (x > 1) {
        // Shift neighbour counts down
        neighbours[0] = neighbours[1];
        neighbours[1] = neighbours[2];
      }

      const auto nextX = x + 1;
      neighbours[2] = neighboursBelow[nextX] + neighboursMiddle[nextX] +
                      neighboursAbove[nextX];

      // Compute new cell state
      const auto totalNeighbours =
          neighbours[0] + neighbours[1] + neighbours[2];

      if (currentStateBool && (totalNeighbours < 3 || totalNeighbours > 4)) {
        output[i] = DEAD;
      } else if (!currentStateBool && totalNeighbours == 3) {
        output[i] = ALIVE;
      } else {
        output[i] = currentStateBool;
      }
    }
  }
}

void nextBoard(const BoardMeta& board) {
  const auto& width = board.width;
  const auto& height = board.height;
  const auto& input = board.input;
  const auto& output = board.output;

  const auto totalThreads = std::min(board.threads, (uint)height);
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
        std::thread([startY, endY, width, input, output]() {
          nextBoardSection(startY, endY, width, input, output);
        }));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  padding(board.output, board.width, board.height);
}
