#pragma once

#include "board.h"
#include <thread>

Board nextBoard(const Board &input);

int getThreads();
void setThreads(int n);
