#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <array>
#include "../board/next.h"
#include "../board/padding.h"

using BoardVector = std::vector<std::vector<bool>>;

void generate(BoardMeta& board, BoardVector vector, bool a = true) {
  const auto height = (unsigned int)vector.size();
  const auto width = (unsigned int)vector[0].size();

  board.resize(width, height);
  for (unsigned int y = 1; y < height+1; ++y)
    for (unsigned int x = 1; x < width+1; ++x)
      board.input[y*(width+2) + x] = vector[y-1][x-1] ? ALIVE : DEAD;

  padding(board.input, width, height);

  if (!a)
    board.flip();
}

BoardVector ungenerate(BoardMeta& board) {
  const auto& output = board.output;
  const auto& width = board.width;
  const auto& height = board.height;

  std::vector<std::vector<bool>> vector(height, std::vector<bool>(width));
  for (unsigned int y = 1; y < height+1; ++y)
    for (unsigned int x = 1; x < width+1; ++x)
      vector[y-1][x-1] = output[y * (width+2) + x] == ALIVE ? 1 : 0;

  return vector;
}

void compare(BoardVector a, BoardVector b) {
  auto boardA = BoardMeta(1,1);
  auto boardB = BoardMeta(1,1);
  generate(boardA, a, true);
  generate(boardB, b, false);
  nextBoard(boardA);
  REQUIRE(ungenerate(boardA) == ungenerate(boardB));
}

TEST_CASE("nothing", "[nextBoard]") {
  compare({{false}}, {{false}});
}

TEST_CASE("death", "[nextBoard]") {
  compare({{true}}, {{false}});
}

TEST_CASE("block", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}});
}

TEST_CASE("block (vertical wrap)", "[nextBoard]") {
  compare(
      {{0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}},
      {{0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}});
}

TEST_CASE("block (horizontal wrap)", "[nextBoard]") {
  compare(
      {{0, 0, 0}, {1, 0, 1}, {1, 0, 1}, {0, 0, 0}},
      {{0, 0, 0}, {1, 0, 1}, {1, 0, 1}, {0, 0, 0}});
}

TEST_CASE("block (corner wrap)", "[nextBoard]") {
  compare({{1, 0, 1}, {0, 0, 0}, {1, 0, 1}}, {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}});
}

TEST_CASE("bee-hive", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 0, 0, 0, 0, 0}});
}

TEST_CASE("loaf", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 0, 1, 0},
       {0, 0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 0, 1, 0},
       {0, 0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0, 0}});
}

TEST_CASE("boat", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}});
}

TEST_CASE("tub", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}});
}

TEST_CASE("blinker 1", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 1, 1, 1, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}});
}

TEST_CASE("blinker 2", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 1, 1, 1, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}});
}

TEST_CASE("toad 1", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 1, 0},
       {0, 1, 1, 1, 0, 0},
       {0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0, 0},
       {0, 0, 0, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 0, 0, 0},
       {0, 0, 0, 0, 0, 0}});
}

TEST_CASE("toad 2", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 0, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 0, 0, 0},
       {0, 0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 1, 0},
       {0, 1, 1, 1, 0, 0},
       {0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0}});
}

TEST_CASE("beacon 1", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 1, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0, 0}});
}

TEST_CASE("beacon 2", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 1, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0, 0}});
}

TEST_CASE("glider 1", "[nextBoard]") {
  compare(
      {{1, 0, 0, 0, 0},
       {0, 1, 1, 0, 0},
       {1, 1, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}},

      {{0, 1, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {1, 1, 1, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}});
}

TEST_CASE("glider 2", "[nextBoard]") {
  compare(
      {{0, 1, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {1, 1, 1, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0},
       {1, 0, 1, 0, 0},
       {0, 1, 1, 0, 0},
       {0, 1, 0, 0, 0},
       {0, 0, 0, 0, 0}});
}

TEST_CASE("glider 3", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 1, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0},
       {0, 0, 0, 1, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0}});
}

TEST_CASE("glider 4", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 1, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 1, 1},
       {0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0}});
}

TEST_CASE("gliders wide horizontal wrap collision [long board]", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
       {1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
       {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
       {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
       {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
}

TEST_CASE("glider tall vertical wrap collision [tall board]", "[nextBoard]") {
  compare(
      {{0, 0, 0, 1, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 1, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0}},

      {{0, 0, 1, 0, 0},
       {0, 0, 0, 1, 1},
       {0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 1, 1},
       {0, 0, 1, 1, 0},
       {0, 0, 1, 1, 0}});
}

TEST_CASE("2x2", "[padding]") {
  std::vector<Cell>input{
    0, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 0,
  };
  std::vector<Cell>expected{
    1, 0, 1, 0,
    0, 1, 0, 1,
    1, 0, 1, 0,
    0, 1, 0, 1,
  };
  padding(&input[0], 2, 2);
  REQUIRE(input == expected);
}

TEST_CASE("4x3", "[padding]") {
  std::vector<Cell>input{
    0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
  };
  std::vector<Cell>expected{
    0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1,
  };
  padding(&input[0], 4, 3);
  REQUIRE(input == expected);
}
