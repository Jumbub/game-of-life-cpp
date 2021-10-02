#pragma once

#include <array>
#include <tuple>
#include <array>
#include <memory>
#include <SDL2/SDL.h>

using Cell = Uint32;
const Cell ALIVE = SDL_MAX_UINT32;
const Cell DEAD = 0;

using Board = std::tuple<std::shared_ptr<Cell[]>, int, int>;
using NeighbourPositions = std::array<int, 8>;
