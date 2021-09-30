#pragma once

#include "board.h"
#include <vector>

Board randomBoard(int width, int height);

Board benchmarkBoard(int width, int height);

Board fromVector(std::vector<std::vector<bool>> board);
