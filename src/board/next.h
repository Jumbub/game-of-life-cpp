#pragma once

#include <thread>
#include "board.h"

void nextBoard(const BoardMeta& input);

unsigned int getThreads();
void setThreads(unsigned int n);
