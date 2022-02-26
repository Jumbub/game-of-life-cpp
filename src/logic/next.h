#pragma once

#include <thread>
#include "board.h"

inline void revokeSkipForNeighbours(const uint& i, Cell* skips, const uint& realWidth);

void nextBoard(Board& board, const uint& threadCount);

std::thread startNextBoardLoopThread(
    const ulong& maxGenerations,
    const uint& threadCount,
    Board& board,
    ulong& computedGenerations);
