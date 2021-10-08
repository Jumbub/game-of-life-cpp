#pragma once

#include <vector>
#include "board.h"

BoardMeta benchmarkBoard(unsigned int width, unsigned int height);

BoardMeta fromVector(std::vector<std::vector<bool>> board);
