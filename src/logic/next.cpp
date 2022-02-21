#include <algorithm>
#include <array>
#include <cstring>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
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

  const auto a = top[0] + middle[0] + bottom[0];
  const auto b = top[1] + middle[1] * 9 + bottom[1];
  const auto c = top[2] + middle[2] + bottom[2];

  return LOOKUP[a + b + c];
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

    output[i] = isAlive(i, input, realWidth);

    if (input[i] != output[i]) {
      revokeSkipForNeighbours(i, outSkip, realWidth);
    }

    i++;
  }
}

void nextBoard(Board& board, const uint& threadCount) {
  board.setOutputToInput();

  std::memset(board.outSkip, true, sizeof(Cell) * board.rawSize);

  std::vector<std::thread> threads;
  const uint segments = board.height / threadCount;
  const uint remainder = board.height % threadCount;
  for (uint t = 0; t < threadCount; t++) {
    threads.push_back(std::thread([&, t]() {
      const uint startY = segments * t + 1;
      const uint finishY = startY + segments + ((t == threadCount - 1) * remainder);

      const uint startI = startY * board.rawWidth + 1;
      const uint finishI = finishY * board.rawWidth - 1;

      board.inSkip[finishI] = false;  // Never skip last cell

      nextBoardSection(startI, finishI, board.rawWidth, board.input, board.output, board.inSkip, board.outSkip);
    }));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  assignBoardPadding(board);
}
