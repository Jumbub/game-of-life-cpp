#include "next.h"
#include <algorithm>
#include <array>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>
#include "../util/profile.h"

auto THREAD_COUNT =
    std::max(std::thread::hardware_concurrency(), (unsigned int)1);

unsigned int getThreads() {
  return THREAD_COUNT;
}

void setThreads(unsigned int n) {
  THREAD_COUNT = std::max(n, (unsigned int)1);
}

void nextBoardSection(
    const unsigned int startY,
    const unsigned int endY,
    const BoardMeta& board) {
  const auto &input = board.input;
  const auto &output = board.output;
  const auto &width = board.width;
  const auto &height = board.height;

  unsigned int neighbours[3] = {0, 0, 0};
  unsigned int nextYBase = 0;
  unsigned int middleYBase = 0;
  unsigned int lastYBase = 0;

  Cell* neighboursBelow = nullptr;
  Cell* neighboursMiddle = nullptr;
  Cell* neighboursAbove = nullptr;

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
      neighbours[1] = neighboursBelow[x] + neighboursAbove[x] + currentStateBool;
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
    if (currentStateBool && (totalNeighbours < 3 || totalNeighbours > 4))
      output[i] = DEAD;
    else if (!currentStateBool && totalNeighbours == 3)
      output[i] = ALIVE;
    else
      output[i] = currentStateBool;
  }
}

void nextBoard(const BoardMeta& board) {
  const auto &height = board.height;

  auto totalThreads = std::min(getThreads(), (unsigned int)height);
  auto threadLines = height / totalThreads;
  auto threadLinesRemaining = height % totalThreads;

  std::vector<std::thread> threads;
  for (unsigned int t = 0; t < totalThreads; t++) {
    // Compute start and end indexes for threads
    const auto startY = t * threadLines;
    auto endY = (t + 1) * threadLines;

    // In the case of an uneven divide, the last thread gets the left-overs
    if (t == totalThreads - 1)
      endY += threadLinesRemaining;

    threads.push_back(
        std::thread(&nextBoardSection, startY, endY, board));
  }

  for (auto& thread : threads) {
    thread.join();
  }
}
