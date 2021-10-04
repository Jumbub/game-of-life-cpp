#include "next.h"
#include "../util/profile.h"
#include <algorithm>
#include <array>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>

auto THREAD_COUNT =
    std::max(std::thread::hardware_concurrency(), (unsigned int)1);

unsigned int getThreads() { return THREAD_COUNT; }
void setThreads(unsigned int n) { THREAD_COUNT = std::max(n, (unsigned int)1); }

inline const auto maxToOne(Cell n) { return (1 - (n + 1)); }

void nextBoardSection(const unsigned int startY, const unsigned int endY, const Board &board,
                      Cell *output) {
  const auto &[input, width, height] = board;

  unsigned int neighbours[3] = {UINT32_MAX, UINT32_MAX, UINT32_MAX};
  unsigned int yAboveBase = 0;
  unsigned int yBelowBase = 0;
  unsigned int yBase = 0;

  const auto endI = endY * width;
  for (unsigned int i = startY * width; i < endI; i++) {
    const unsigned int x = i % width;
    auto currentStateBool = maxToOne(input[i]);

    // Slide neighbours
    if (x == 0) {
      // Clear neighbour columns
      neighbours[0] = UINT32_MAX;
      neighbours[1] = UINT32_MAX;

      // Compute new Y levels
      const unsigned int y = i / width;
      yBase = y * width;
      yBelowBase = ((y - 1 + height) % height) * width;
      yAboveBase = ((y + 1) % height) * width;
    } else {
      neighbours[0] = neighbours[1];
      // Remove own value
      neighbours[1] = neighbours[2] - currentStateBool;
    }

    // Compute neighbours
    if (neighbours[0] == UINT32_MAX) {
      const auto previousX = (x - 1 + width) % width;

      neighbours[0] = maxToOne(input[yBelowBase + previousX]) +
                      maxToOne(input[yBase + previousX]) +
                      maxToOne(input[yAboveBase + previousX]);
    }
    if (neighbours[1] == UINT32_MAX) {
      neighbours[1] =
          maxToOne(input[yBelowBase + x]) + maxToOne(input[yAboveBase + x]);
    }
    const auto nextX = (x + 1) % width;
    neighbours[2] = maxToOne(input[yBelowBase + nextX]) +
                    maxToOne(input[yBase + nextX]) +
                    maxToOne(input[yAboveBase + nextX]);

    // Compute new cell state
    const auto totalNeighbours = neighbours[0] + neighbours[1] + neighbours[2];
    if (currentStateBool && (totalNeighbours < 2 || totalNeighbours > 3))
      output[i] = DEAD;
    else if (!currentStateBool && totalNeighbours == 3)
      output[i] = ALIVE;
    else
      output[i] = input[i];

    // Add self to neighbours count
    neighbours[1] += currentStateBool;
  }
}

Board nextBoard(const Board &board) {
  const auto &[input, width, height] = board;
  auto output = new Cell[width * height];

  auto totalThreads = std::min(getThreads(), height);
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
        std::thread(&nextBoardSection, startY, endY, board, output));
  }

  for (auto &thread : threads) {
    thread.join();
  }

  delete input;

  return {output, width, height};
}
