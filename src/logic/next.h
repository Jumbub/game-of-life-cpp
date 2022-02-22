#pragma once

#include <thread>
#include "board.h"

void nextBoard(Board& board, const uint& threadCount);

std::thread startNextBoardLoopThread(
    const ulong& maxGenerations,
    const uint& threadCount,
    Board& board,
    ulong& computedGenerations);
