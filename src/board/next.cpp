#include "next.h"
#include "../util/profile.h"
#include <algorithm>
#include <array>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>

void nextBoardSection(const int startY, const int endY, const Board &board,
                       Cell *output) {
  const auto &[input, width, height] = board;

  int neighbours[3];
  int yAboveBase;
  int yBelowBase;
  int yBase;

  const auto endI = endY * width;
  for (int i = startY * width; i < endI; i++) {
    const int x = i % width;
    auto currentStateBool = input[i] == ALIVE ? 1 : 0;

    // Slide neighbours
    if (x == 0) {
      // Clear neighbour columns
      neighbours[0] = -1;
      neighbours[1] = -1;

      // Compute new Y levels
      const int y = i / width;
      yBase = y * width;
      yBelowBase = ((y - 1 + height) % height) * width;
      yAboveBase = ((y + 1) % height) * width;
    } else {
      neighbours[0] = neighbours[1];
      // Remove own value
      neighbours[1] = neighbours[2] - currentStateBool;
    }

    // Compute neighbours
    if (neighbours[0] == -1) {
      const auto previousX = (x - 1 + width) % width;

      neighbours[0] = (input[yBelowBase + previousX] ? 1 : 0) +
                      (input[yBase + previousX] ? 1 : 0) +
                      (input[yAboveBase + previousX] ? 1 : 0);
    }
    if (neighbours[1] == -1) {
      neighbours[1] =
          (input[yBelowBase + x] ? 1 : 0) + (input[yAboveBase + x] ? 1 : 0);
    }
    const auto nextX = (x + 1) % width;
    neighbours[2] = (input[yBelowBase + nextX] ? 1 : 0) +
                    (input[yBase + nextX] ? 1 : 0) +
                    (input[yAboveBase + nextX] ? 1 : 0);

    // Compute new cell state
    const int totalNeighbours = neighbours[0] + neighbours[1] + neighbours[2];
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

int THREAD_COUNT =
    std::max(std::thread::hardware_concurrency(), (unsigned int)1);

int getThreads() { return THREAD_COUNT; }
void setThreads(int n) { THREAD_COUNT = std::max(n, 1); }

Board nextBoard(const Board &board) {
  const auto &[input, width, height] = board;
  auto output = new Cell[width * height];
  const auto threads = getThreads();

  const auto split = height / std::min(height, (int)threads);
  const auto remainder = height % threads;

  std::thread nextBoardSegments[threads];
  for (int thread = 0; thread < threads; thread++) {
    // Compute start and end indexes for threads
    const auto startY = thread * split;
    auto endY = (thread + 1) * split;

    // In the case of an uneven divide, the last thread gets the left-overs
    if (thread == threads - 1)
      endY += remainder;

    nextBoardSegments[thread] =
        std::thread(&nextBoardSection, startY, endY, board, output);
  }

  for (int i = 0; i < threads; i++) {
    nextBoardSegments[i].join();
  }

  free(input);

  return {output, width, height};
}
