#pragma once

#include <functional>
#include <thread>
#include <vector>
#include "board.h"

using Jobs = std::vector<std::function<void()>>;
struct Segment {
  uint begin;
  uint end;
};
using Segments = std::vector<Segment>;

Segments createSegments(const Board& board, const uint& jobCount);
Jobs createJobs(const Board& board, const Segments& segments, const uint& jobCount);

void nextBoard(Board& board, const uint& threadCount, const uint& jobCount, const Segments& segments, const Jobs& jobs);

std::thread startNextBoardLoopThread(
    const ulong& maxGenerations,
    const uint& threadCount,
    const uint& jobCount,
    Board& board,
    ulong& computedGenerations);
