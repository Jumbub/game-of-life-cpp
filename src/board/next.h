#pragma once

#include <algorithm>
#include <array>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>
#include "../util/profile.h"
#include "board.h"
#include "threads.h"

void nextBoardSection(
    const unsigned int startY,
    const unsigned int endY,
    const uint width,
    const uint height,
    const Cell* input,
    Cell* output,
    [[maybe_unused]]
    Cell* render) {
  unsigned int neighbours[3] = {0, 0, 0};
  unsigned int nextYBase = 0;
  unsigned int middleYBase = 0;
  unsigned int lastYBase = 0;

  const Cell* neighboursBelow = nullptr;
  const Cell* neighboursMiddle = nullptr;
  const Cell* neighboursAbove = nullptr;

  const auto endI = endY * width;
  for (unsigned int i = startY * width; i < endI; i++) {
    const unsigned int x = i % width;
    auto currentStateBool = input[i];

    // Slide neighbours
    if (x == 0) {
      // Compute new Y levels
      const unsigned int y = i / width;
      lastYBase = ((y - 1 + height) % height) * width;
      middleYBase = y * width;
      nextYBase = ((y + 1) % height) * width;

      neighboursBelow = &input[lastYBase];
      neighboursMiddle = &input[middleYBase];
      neighboursAbove = &input[nextYBase];

      // Left neighbours
      const auto previousX = (x - 1 + width) % width;
      neighbours[0] = neighboursBelow[previousX] + neighboursMiddle[previousX] +
                      neighboursAbove[previousX];

      // Middle neighbours
      neighbours[1] =
          neighboursBelow[x] + neighboursAbove[x] + currentStateBool;
    } else {
      // Shift neighbour counts down
      neighbours[0] = neighbours[1];
      neighbours[1] = neighbours[2];
    }

    const auto nextX = (x + 1) % width;
    neighbours[2] = neighboursBelow[nextX] + neighboursMiddle[nextX] +
                    neighboursAbove[nextX];

    // Compute new cell state
    const auto totalNeighbours = neighbours[0] + neighbours[1] + neighbours[2];
    if (currentStateBool && (totalNeighbours < 3 || totalNeighbours > 4)) {
      output[i] = DEAD;
      render[i] = DEAD_RENDER;
    } else if (!currentStateBool && totalNeighbours == 3) {
      output[i] = ALIVE;
      render[i] = ALIVE_RENDER;
    } else if (currentStateBool) {
      output[i] = ALIVE;
      render[i] = ALIVE_RENDER;
    } else {
      output[i] = DEAD;
      render[i] = DEAD_RENDER;
    }
  }
}

void nextBoard(const BoardMeta& board) {
  const auto& width = board.width;
  const auto& height = board.height;
  const auto& input = board.input;
  const auto& output = board.output;
  const auto& render = board.render;

  const auto totalThreads = std::min(board.threads, (unsigned int)height);
  const auto threadLines = height / totalThreads;
  const auto threadLinesRemaining = height % totalThreads;

  std::vector<std::thread> threads;
  for (unsigned int t = 0; t < totalThreads; t++) {
    // Compute start and end indexes for threads
    const auto startY = t * threadLines;
    auto endY = (t + 1) * threadLines;

    // In the case of an uneven divide, the last thread gets the left-overs
    if (t == totalThreads - 1)
      endY += threadLinesRemaining;

    threads.push_back(
        std::thread([startY, endY, width, height, input, output, render]() {
          nextBoardSection(startY, endY, width, height, input, output, render);
        }));
  }

  for (auto& thread : threads) {
    thread.join();
  }
}
