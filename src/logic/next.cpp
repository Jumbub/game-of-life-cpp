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
  const Cell* top = &input[INDEX(i - realWidth - 1)];
  const Cell* middle = &input[INDEX(i - 1)];
  const Cell* bottom = &input[INDEX(i + realWidth - 1)];

  const auto a = top[0] + middle[0] + bottom[0];
  const auto b = top[INDEX(1)] + middle[INDEX(1)] * 9 + bottom[INDEX(1)];
  const auto c = top[INDEX(2)] + middle[INDEX(2)] + bottom[INDEX(2)];

  return LOOKUP[a + b + c];

  /* std::cout << "-" << realWidth << std::endl; */
  /* std::cout << "-" << INDEX(7 - realWidth - 1) << std::endl; */
  /* std::cout << "___" << std::endl; */
  /* std::cout << i << std::endl; */
  /* std::cout << i * 2 << std::endl; */
  /* std::cout << INDEX(i - realWidth - 1) << std::endl; */
  /* std::cout << INDEX(i - 1) << std::endl; */
  /* std::cout << INDEX(i + realWidth - 1) << std::endl; */

  /* std::cout << "___" << std::endl; */
  /* std::cout << INDEX(i) << std::endl; */
  /* std::cout << a + b + c << std::endl; */

  /* return LOOKUP */
  /*     [input[INDEX(i - realWidth - 1)] + input[INDEX(i - realWidth)] + input[INDEX(i - realWidth + 1)] + */

  /*      input[INDEX(i - 1)] + input[INDEX(i)] * 9 + input[INDEX(i + 1)] + */

  /*      input[INDEX(i + realWidth - 1)] + input[INDEX(i + realWidth)] + input[INDEX(i + realWidth + 1)]]; */
}

inline void revokeSkipForNeighbours(const uint& i, Cell* skips, const uint& realWidth) {
  *reinterpret_cast<uint32_t*>(&skips[i - realWidth - 1]) = false;
  *reinterpret_cast<uint32_t*>(&skips[i - 1]) = false;
  *reinterpret_cast<uint32_t*>(&skips[i + realWidth - 1]) = false;
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
    while (uint8s_to_uint64(&inSkip[i]) == SKIP_EIGHT)
      i += 8;

    output[INDEX(i)] = isAlive(i, input, realWidth);

    if (input[INDEX(i)] != output[INDEX(i)]) {
      revokeSkipForNeighbours(i, outSkip, realWidth);
    }

    i++;
  }
}

void nextBoard(Board& board, const uint& threadCount, const uint& jobCount) {
  board.setOutputToInput();

  std::memset(board.outSkip, true, sizeof(Cell) * board.rawSize);

  const uint segmentSize = (board.height / jobCount + board.height % jobCount) * board.rawWidth;
  uint endI = board.rawWidth + 1;

  std::vector<std::thread> threads(threadCount);

  std::vector<std::function<void()>> jobs(jobCount);
  std::atomic<uint> job = {0};

  for (auto& job : jobs) {
    const uint beginI = endI;
    endI = std::min(board.rawSize - board.rawWidth, endI + segmentSize);
    board.inSkip[endI] = false;  // Never skip last cell

    job = [&, beginI, endI]() {
      nextBoardSection(beginI, endI, board.rawWidth, board.input, board.output, board.inSkip, board.outSkip);
    };
  };

  for (auto& thread : threads) {
    thread = std::thread([&]() {
      uint current = job.fetch_add(1);
      while (current < jobCount) {
        jobs[current]();
        current = job.fetch_add(1);
      }
    });
  };

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
