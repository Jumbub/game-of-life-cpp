#include <cstring>
#include <future>
#include <iostream>
#include <thread>
#include <vector>
#include "board.h"
#include "next.h"
#include "padding.h"
#include "threads.h"

inline uint64_t uint8s_to_uint64(Cell* ns) {
  return *reinterpret_cast<uint64_t*>(ns);
}

constexpr uint8_t LOOKUP[20] = {
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
};

const uint64_t SKIP_EIGHT =
    (1 << 0) + (1 << 8) + (1 << 16) + (1 << 24) + (1l << 32) + (1l << 40) + (1l << 48) + (1l << 56);

uint isAlive(const uint& i, const Cell* input, const uint& realWidth) {
  const Cell* top = &input[i - realWidth - 1];
  const Cell* middle = &input[i - 1];
  const Cell* bottom = &input[i + realWidth - 1];

  const auto a = testAlive(top[0]) + testAlive(middle[0]) + testAlive(bottom[0]);
  const auto b = testAlive(top[1]) + testAlive(middle[1]) * 9 + testAlive(bottom[1]);
  const auto c = testAlive(top[2]) + testAlive(middle[2]) + testAlive(bottom[2]);

  return LOOKUP[a + b + c];
}

inline void revokeSkipForNeighbours(const uint& i, Cell* output, const uint& realWidth) {
  resetSkip(output[i - realWidth - 1]);
  resetSkip(output[i - realWidth]);
  resetSkip(output[i - realWidth + 1]);
  resetSkip(output[i - 1]);
  resetSkip(output[i]);
  resetSkip(output[i + 1]);
  resetSkip(output[i + realWidth - 1]);
  resetSkip(output[i + realWidth]);
  resetSkip(output[i + realWidth + 1]);
}

void nextBoardSection(uint i, const uint endI, const uint realWidth, Cell* input, Cell* output) {
  while (i < endI) {
    while ((uint8s_to_uint64(&input[i]) & SKIP_EIGHT) == SKIP_EIGHT)
      i += 8;

    output[i] = isAlive(i, input, realWidth);

    if (input[i] != output[i]) {
      revokeSkipForNeighbours(i, output, realWidth);
    }

    i++;
  }
}

void nextBoard(Board& board, const uint& threadCount, const uint& jobCount) {
  board.setOutputToInput();

  // Create segments

  std::vector<std::tuple<uint, uint>> segments(jobCount);

  const uint segmentSize = (board.height / jobCount + board.height % jobCount) * board.rawWidth;
  uint endI = board.rawWidth;
  for (auto& segment : segments) {
    const uint beginI = endI;
    endI = std::min(board.rawSize - board.rawWidth, endI + segmentSize);
    segment = {beginI, endI};
  };

  // Create jobs

  std::vector<std::function<void()>> jobs(jobCount);

  for (uint i = 0; i < jobCount; i++) {
    const auto& beginI = std::get<0>(segments[i]);
    const auto& endI = std::get<1>(segments[i]);

    resetSkip(board.input[endI]);  // Never skip last cell

    // Reset next border
    if (i == jobCount - 1) {
      for (uint i = endI - board.rawWidth; i < board.rawSize - (endI - board.rawWidth); i++) {
        setSkip(board.output[i]);
      }
    } else {
      const auto borderSize = std::min(board.rawSize, endI + board.rawWidth * 3);
      for (uint i = endI - board.rawWidth; i < borderSize - endI; i++) {
        setSkip(board.output[i]);
      }
    }

    jobs[i] = [&, beginI, endI]() {
      // Reset inner skips
      if (endI == beginI)
        return;
      const auto max = std::max(beginI + board.rawWidth, endI - board.rawWidth);
      for (uint i = beginI + board.rawWidth; i < max - beginI - board.rawWidth; i++) {
        setSkip(board.output[i]);
      }
      nextBoardSection(beginI + 1, endI - 1, board.rawWidth, board.input, board.output);
    };
  };

  // Reset first border
  for (uint i = 0; i < board.rawWidth; i++) {
    setSkip(board.output[i]);
  }

  std::atomic<uint> job = {0};
  std::vector<std::thread> threads(threadCount - 1);
  for (auto& thread : threads) {
    thread = std::thread([&]() {
      uint current = job.fetch_add(1);
      while (current < jobCount) {
        jobs[current]();
        current = job.fetch_add(1);
      }
    });
  };

  uint current = job.fetch_add(1);
  while (current < jobCount) {
    jobs[current]();
    current = job.fetch_add(1);
  }

  for (auto& thread : threads) {
    thread.join();
  }

  assignBoardPadding(board);
}

std::thread startNextBoardLoopThread(
    const ulong& maxGenerations,
    const uint& threadCount,
    const uint& jobCount,
    Board& board,
    ulong& computedGenerations) {
  return std::thread{[&]() {
    while (computedGenerations < maxGenerations) {
      board.lock.pauseIfRequested();

      nextBoard(board, threadCount, jobCount);
      ++computedGenerations;
    }
  }};
}
