#pragma once

#include "board.h"
#include <thread>

Board nextBoard(const Board &input);

unsigned int getThreads();
void setThreads(unsigned int n);
