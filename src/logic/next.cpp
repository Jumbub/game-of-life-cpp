#include <cstring>
#include <future>
#include <iostream>
#include <thread>
#include <vector>
#include "board.h"
#include "next.h"
#include "padding.h"
#include "threads.h"

constexpr uint8_t LOOKUP[20] = {
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
};

void nextBoardSection(
    uint i,
    const uint endI,
    const uint paddedWidth,
    const boost::dynamic_bitset<>& input,
    boost::dynamic_bitset<>& output,
    const boost::dynamic_bitset<>& jobs,
    boost::dynamic_bitset<>& nextJobs) {
  do {
    // Compute alive state
    const auto neighbours =
        input.test(i - paddedWidth - 1) + input.test(i - paddedWidth) + input.test(i - paddedWidth + 1)

        + input.test(i - 1) + input.test(i) * 9 + input.test(i + 1)

        + input.test(i + paddedWidth - 1) + input.test(i + paddedWidth) + input.test(i + paddedWidth + 1);
    output[i] = LOOKUP[neighbours];

    // If state changed, mark as non-skippable
    if (input[i] != output[i]) {
      // TODO: check out if you can remove the size check assertions
      nextJobs.set(i - paddedWidth - 1, COMPUTE);
      nextJobs.set(i - paddedWidth, COMPUTE);
      nextJobs.set(i - paddedWidth + 1, COMPUTE);
      nextJobs.set(i - 1, COMPUTE);
      nextJobs.set(i, COMPUTE);
      nextJobs.set(i + 1, COMPUTE);
      nextJobs.set(i + paddedWidth - 1, COMPUTE);
      nextJobs.set(i + paddedWidth, COMPUTE);
      nextJobs.set(i + paddedWidth + 1, COMPUTE);
    }

    i = jobs.find_next(i);  // Find next non-skippable (1)
  } while (i < endI);
}

void nextBoard(Board& board, const uint& threadCount, const uint& jobCount) {
  board.input.swap(board.output);
  board.jobs.swap(board.nextJobs);

  assignBoardPadding(board);

  board.nextJobs.reset();
  /* board.jobs.set(); */

  // Create segments

  std::vector<std::tuple<uint, uint>> segments(jobCount);

  const uint segmentSize = (board.height / jobCount + board.height % jobCount) * board.paddedWidth;
  uint endI = board.paddedWidth;
  for (auto& segment : segments) {
    const uint beginI = endI;
    endI = std::min(board.paddedSize - board.paddedWidth, endI + segmentSize);
    segment = {beginI, endI};
  };

  // Create jobs

  std::vector<std::function<void()>> jobs(jobCount);

  for (uint i = 0; i < jobCount; i++) {
    const auto& beginI = std::get<0>(segments[i]);
    const auto& endI = std::get<1>(segments[i]);

    jobs[i] = [&, beginI, endI]() {
      nextBoardSection(beginI + 1, endI - 1, board.paddedWidth, board.input, board.output, board.jobs, board.nextJobs);
    };
  };

  // Start workers

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

  // Join threads

  for (auto& thread : threads) {
    thread.join();
  }
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
