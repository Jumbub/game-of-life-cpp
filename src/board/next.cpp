#include "next.h"
#include "../util/profile.h"
#include <algorithm>
#include <array>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>

int THREAD_COUNT =
    std::max(std::thread::hardware_concurrency(), (unsigned int)1);

int getThreads() { return THREAD_COUNT; }
void setThreads(int n) { THREAD_COUNT = std::max(n, 1); }

void nextBoardSection(const int startY, const int endY, const Board &board,
                      Cell *output) {
  const auto &[input, width, height] = board;

  int neighbours[3] = {0,0,0};
  int yAboveBase = 0;
  int yBelowBase = 0;
  int yBase = 0;

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

      neighbours[0] = (1 - (input[yBelowBase + previousX] + 1)) +
                      (1 - (input[yBase + previousX] + 1)) +
                      (1 - (input[yAboveBase + previousX] + 1));
    }
    if (neighbours[1] == -1) {
      neighbours[1] =
          (1 - (input[yBelowBase + x] + 1)) + (1 - (input[yAboveBase + x] + 1));
    }
    const auto nextX = (x + 1) % width;
    neighbours[2] = (1 - (input[yBelowBase + nextX] + 1)) +
                    (1 - (input[yBase + nextX] + 1)) +
                    (1 - (input[yAboveBase + nextX] + 1));

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

Board nextBoard(const Board &board) {
  const auto &[input, width, height] = board;
  auto output = new Cell[width * height];

  auto totalThreads = std::min(getThreads(), height);
  auto threadLines = height / totalThreads;
  auto threadLinesRemaining = height % totalThreads;

  std::vector<std::thread> threads;
  for (int t = 0; t < totalThreads; t++) {
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

  free(input);

  return {output, width, height};
}
