#pragma once

#include <array>
#include <memory>
#include <tuple>

using Cell = unsigned int;
[[ maybe_unused ]]
const Cell ALIVE = 1;
[[ maybe_unused ]]
const Cell DEAD = 0;

struct BoardMeta {
  Cell* input;
  Cell* output;
  unsigned int width;
  unsigned int height;
};
