#pragma once

#include <array>
#include <tuple>
#include <vector>

using Board = std::vector<std::vector<bool>>;
using Position = std::tuple<int, int>;
using NeighbourPositions = std::array<Position, 8>;
