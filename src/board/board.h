#pragma once

#include <array>
#include <tuple>
#include <array>
#include <memory>

using Cell = unsigned int; // Should match SDL::Uint32, otherwise the rendering will not work as intended
const Cell ALIVE = UINT32_MAX; // Should match SDL::SDL_UINT32_MAX, "
const Cell DEAD = 0;

using Board = std::tuple<Cell*, int, int>;
using NeighbourPositions = std::array<int, 8>;
