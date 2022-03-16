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

constexpr uint8_t SKIP_EIGHT = 1;

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
  const auto t = i - realWidth;
  const auto m = i;
  const auto b = i + realWidth;

  const auto top = t / 8;
  const auto middle = m / 8;
  const auto bottom = b / 8;
  skips[top] = 0;
  skips[middle] = 0;
  skips[bottom] = 0;

  if (t % 8 == 0)
    skips[top - 1] = 0;
  if (t % 8 == 7)
    skips[top + 1] = 0;

  if (m % 8 == 0)
    skips[middle - 1] = 0;
  if (m % 8 == 7)
    skips[middle + 1] = 0;

  if (b % 8 == 0)
    skips[bottom - 1] = 0;
  if (b % 8 == 7)
    skips[bottom + 1] = 0;
}

void nextBoardSection(
    uint i,
    const uint endI,
    const uint realWidth,
    Cell* input,
    Cell* output,
    uint8_t* inSkip,
    uint8_t* outSkip) {
  while (i < endI) {
    while (inSkip[i / 8] == SKIP_EIGHT && i < endI)
      i++;

    output[i] = isAlive(i, input, realWidth);

    if (input[i] != output[i]) {
      revokeSkipForNeighbours(i, outSkip, realWidth);
    }

    i++;
  }
}

void nextBoard(Board& board, const uint& threadCount, const uint& jobCount) {
  board.setOutputToInput();

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

    board.inSkip[endI] = false;  // Never skip last cell

    // Reset next border
    if (i == jobCount - 1) {
      std::memset(&board.outSkip[endI - board.rawWidth], true, board.rawSize - (endI - board.rawWidth));
    } else {
      const auto borderSize = std::min(board.rawSize, endI + board.rawWidth * 3);
      std::memset(&board.outSkip[endI - board.rawWidth], true, borderSize - endI);
    }

    jobs[i] = [&, beginI, endI]() {
      // Reset inner skips
      if (endI == beginI)
        return;
      const auto max = std::max(beginI + board.rawWidth, endI - board.rawWidth);
      std::memset(&board.outSkip[beginI + board.rawWidth], true, max - beginI - board.rawWidth);
      nextBoardSection(beginI + 1, endI - 1, board.rawWidth, board.input, board.output, board.inSkip, board.outSkip);
    };
  };

  // Reset first border
  std::memset(board.outSkip, true, sizeof(Cell) * board.rawWidth);

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
