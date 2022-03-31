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

uint isAlive(const uint& i, const Cell* input, const uint& realWidth) {
  // TODO: investigate the float bitshift strategy:
  // 00110000 # a live cell needs 2 or 3 left shifts
  // 00010000 # a dead cell needs 3 left shifts
  const Cell* top = &input[i - realWidth - 1];
  const Cell* middle = &input[i - 1];
  const Cell* bottom = &input[i + realWidth - 1];

  const auto a = top[0] + middle[0] + bottom[0];
  const auto b = top[1] + middle[1] * 9 + bottom[1];
  const auto c = top[2] + middle[2] + bottom[2];

  return LOOKUP[a + b + c];
}

inline void revokeSkipForNeighbours(const uint& i, Cell* skips, const uint& realWidth) {
  // TODO: try storing an always casted uint32_t array of skips on board,
  // maybe the compiler will be able to better optimise this
  *reinterpret_cast<uint32_t*>(&skips[(i - realWidth - 1) / SKIPS_PER_BYTE]) = NO_SKIP_BYTE;
  *reinterpret_cast<uint32_t*>(&skips[(i - 1) / SKIPS_PER_BYTE]) = NO_SKIP_BYTE;
  *reinterpret_cast<uint32_t*>(&skips[(i + realWidth - 1) / SKIPS_PER_BYTE]) = NO_SKIP_BYTE;
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
    // TODO: try removing the == check, and just do truthiness
    // TODO: try removing the uint64, and instead increase skips per byte (less ram usage)
    // TODO: try storing skips as uint64 and only casting down when assigning
    // TODO: try doing i / SKIPS once, then pass to "revoke" function
    while (uint8s_to_uint64(&inSkip[i / SKIPS_PER_BYTE]) == SKIP_EIGHT_BYTES)
      i += SKIPS_PER_BYTE * 8;

    output[i] = isAlive(i, input, realWidth);

    if (input[i] != output[i]) {
      revokeSkipForNeighbours(i, outSkip, realWidth);
    }

    i++;
  }
}

void nextBoard(Board& board, const uint& threadCount, const uint& jobCount) {
  board.setOutputToInput();

  // Create segments

  // TODO: try a non vectored tuple
  std::vector<std::tuple<uint, uint>> segments(jobCount);

  const uint segmentSize = (board.height / jobCount + board.height % jobCount) * board.rawWidth;
  uint endI = board.rawWidth;
  // TODO: for loop?
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

    // Reset next border
    if (i == jobCount - 1) {
      std::memset(
          &board.outSkip[(endI - board.rawWidth) / SKIPS_PER_BYTE], SKIP_BYTE,
          (board.rawSize - (endI - board.rawWidth)) / SKIPS_PER_BYTE);
    } else {
      const auto borderSize = std::min(board.rawSize, endI + board.rawWidth * 3);
      // TODO: this next line was wrong im pretty sure
      std::memset(&board.outSkip[(endI - board.rawWidth)/SKIPS_PER_BYTE], SKIP_BYTE, (borderSize - endI) / SKIPS_PER_BYTE);
    }

    // TODO: what if this wasn't a lamda?
    jobs[i] = [&, beginI, endI]() {
      // Reset inner skips
      if (endI == beginI)
        return;
      const auto max = std::max(beginI + board.rawWidth, endI - board.rawWidth);
      std::memset(
          &board.outSkip[(beginI + board.rawWidth) / SKIPS_PER_BYTE], SKIP_BYTE,
          (max - beginI - board.rawWidth) / SKIPS_PER_BYTE);
      nextBoardSection(beginI + 1, endI - 1, board.rawWidth, board.input, board.output, board.inSkip, board.outSkip);
    };

    // TODO: Does this actually have any impact
    board.inSkip[endI] = NO_SKIP_BYTE;  // Never skip last cell of segment
  };

  // Reset first border
  std::memset(board.outSkip, SKIP_BYTE, (sizeof(Cell) * board.rawWidth) / SKIPS_PER_BYTE);

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
