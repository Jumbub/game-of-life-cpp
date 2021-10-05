#pragma once

#include <thread>
#include "board.h"

Board nextBoard(const Board& input);

unsigned int getThreads();
void setThreads(unsigned int n);
