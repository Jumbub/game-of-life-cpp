#define CATCH_CONFIG_MAIN

#include "../board/next.h"
#include <catch2/catch.hpp>

Board generate(std::vector<std::vector<Cell>> input) {
  const auto height = (int)input.size();
  const auto width = (int)input[0].size();

  auto board = new Cell[width * height];
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
      board[y * width + x] = input[y][x] ? ALIVE : DEAD;

  return {board, width, height};
}

std::vector<std::vector<Cell>> ungenerate(Board board) {
  const auto &[input, width, height] = board;

  std::vector<std::vector<Cell>> output(height, std::vector<Cell>(width));
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
      output[y][x] = input[y * width + x];

  return output;
}

void compare(Board a, Board b) { REQUIRE(ungenerate(a) == ungenerate(b)); }

TEST_CASE("nothing", "[nextBoard]") {
  auto input = generate({{false}});
  auto expected = generate({{false}});

  compare(nextBoard(input), expected);
}

TEST_CASE("death", "[nextBoard]") {
  auto input = generate({{true}});
  auto expected = generate({{false}});

  compare(nextBoard(input), expected);
}

TEST_CASE("block", "[nextBoard]") {
  auto input =
      generate({{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}});
  auto expected =
      generate({{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("block (vertical wrap)", "[nextBoard]") {
  auto input = generate({{0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}});
  auto expected = generate({{0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("block (horizontal wrap)", "[nextBoard]") {
  auto input = generate({{0, 0, 0}, {1, 0, 1}, {1, 0, 1}, {0, 0, 0}});
  auto expected = generate({{0, 0, 0}, {1, 0, 1}, {1, 0, 1}, {0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("block (corner wrap)", "[nextBoard]") {
  auto input = generate({{1, 0, 1}, {0, 0, 0}, {1, 0, 1}});
  auto expected = generate({{1, 0, 1}, {0, 0, 0}, {1, 0, 1}});

  compare(nextBoard(input), expected);
}

TEST_CASE("bee-hive", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 1, 0, 0},
                         {0, 1, 0, 0, 1, 0},
                         {0, 0, 1, 1, 0, 0},
                         {0, 0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0, 0},
                            {0, 0, 1, 1, 0, 0},
                            {0, 1, 0, 0, 1, 0},
                            {0, 0, 1, 1, 0, 0},
                            {0, 0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("loaf", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 1, 0, 0},
                         {0, 1, 0, 0, 1, 0},
                         {0, 0, 1, 0, 1, 0},
                         {0, 0, 0, 1, 0, 0},
                         {0, 0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0, 0},
                            {0, 0, 1, 1, 0, 0},
                            {0, 1, 0, 0, 1, 0},
                            {0, 0, 1, 0, 1, 0},
                            {0, 0, 0, 1, 0, 0},
                            {0, 0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("boat", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0},
                         {0, 1, 1, 0, 0},
                         {0, 1, 0, 1, 0},
                         {0, 0, 1, 0, 0},
                         {0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0},
                            {0, 1, 1, 0, 0},
                            {0, 1, 0, 1, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("tub", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0},
                         {0, 0, 1, 0, 0},
                         {0, 1, 0, 1, 0},
                         {0, 0, 1, 0, 0},
                         {0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 1, 0, 1, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("blinker 1", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0},
                         {0, 0, 1, 0, 0},
                         {0, 0, 1, 0, 0},
                         {0, 0, 1, 0, 0},
                         {0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 1, 1, 1, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("blinker 2", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0},
                         {0, 1, 1, 1, 0},
                         {0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("toad 1", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 1, 1, 0},
                         {0, 1, 1, 1, 0, 0},
                         {0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 1, 0, 0},
                            {0, 1, 0, 0, 1, 0},
                            {0, 1, 0, 0, 1, 0},
                            {0, 0, 1, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("toad 2", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0},
                         {0, 1, 0, 0, 1, 0},
                         {0, 1, 0, 0, 1, 0},
                         {0, 0, 1, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0},
                            {0, 0, 1, 1, 1, 0},
                            {0, 1, 1, 1, 0, 0},
                            {0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("beacon 1", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0, 0},
                         {0, 1, 1, 0, 0, 0},
                         {0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 0, 1, 0},
                         {0, 0, 0, 1, 1, 0},
                         {0, 0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0, 0},
                            {0, 1, 1, 0, 0, 0},
                            {0, 1, 1, 0, 0, 0},
                            {0, 0, 0, 1, 1, 0},
                            {0, 0, 0, 1, 1, 0},
                            {0, 0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("beacon 2", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0, 0},
                         {0, 1, 1, 0, 0, 0},
                         {0, 1, 1, 0, 0, 0},
                         {0, 0, 0, 1, 1, 0},
                         {0, 0, 0, 1, 1, 0},
                         {0, 0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0, 0},
                            {0, 1, 1, 0, 0, 0},
                            {0, 1, 0, 0, 0, 0},
                            {0, 0, 0, 0, 1, 0},
                            {0, 0, 0, 1, 1, 0},
                            {0, 0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("glider 1", "[nextBoard]") {
  auto input = generate({{1, 0, 0, 0, 0},
                         {0, 1, 1, 0, 0},
                         {1, 1, 0, 0, 0},
                         {0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0}});
  auto expected = generate({{0, 1, 0, 0, 0},
                            {0, 0, 1, 0, 0},
                            {1, 1, 1, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("glider 2", "[nextBoard]") {
  auto input = generate({{0, 1, 0, 0, 0},
                         {0, 0, 1, 0, 0},
                         {1, 1, 1, 0, 0},
                         {0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0},
                            {1, 0, 1, 0, 0},
                            {0, 1, 1, 0, 0},
                            {0, 1, 0, 0, 0},
                            {0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("glider 3", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0},
                         {0, 1, 0, 1, 0},
                         {0, 0, 1, 1, 0},
                         {0, 0, 1, 0, 0},
                         {0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0},
                            {0, 0, 0, 1, 0},
                            {0, 1, 0, 1, 0},
                            {0, 0, 1, 1, 0},
                            {0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}

TEST_CASE("glider 4", "[nextBoard]") {
  auto input = generate({{0, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0},
                         {0, 1, 0, 1, 0},
                         {0, 0, 1, 1, 0},
                         {0, 0, 0, 0, 0}});
  auto expected = generate({{0, 0, 0, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 0, 1, 1},
                            {0, 0, 1, 1, 0},
                            {0, 0, 0, 0, 0}});

  compare(nextBoard(input), expected);
}
