#pragma once

#include <array>
#include <tuple>
#include <array>
#include <memory>

using Board = std::tuple<std::shared_ptr<bool[]>, int, int>;
using NeighbourPositions = std::array<int, 8>;
