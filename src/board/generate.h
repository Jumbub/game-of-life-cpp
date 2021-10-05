#pragma once

#include <vector>
#include "board.h"

Board randomBoard(int width, int height);

Board benchmarkBoard(int width, int height);

Board fromVector(std::vector<std::vector<bool>> board);
