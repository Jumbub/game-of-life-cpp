#pragma once

#include <array>
#include <memory>
#include <tuple>

using Cell = unsigned int;
const Cell ALIVE = 1;
const Cell DEAD = 0;

using Board = std::tuple<Cell*, unsigned int, unsigned int>;
