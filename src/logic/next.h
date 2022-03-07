#pragma once

#include <thread>
#include "board.h"

void nextBoard(Board& board, const uint& threadCount, const uint& jobCount);

std::thread startNextBoardLoopThread(
    const ulong& maxGenerations,
    const uint& threadCount,
    const uint& jobCount,
    Board& board,
    ulong& computedGenerations);
