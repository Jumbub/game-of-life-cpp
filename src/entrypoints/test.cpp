#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../board/next.h"

BoardMeta generate(std::vector<std::vector<Cell>> vector, bool a = true) {
  const auto height = (unsigned int)vector.size();
  const auto width = (unsigned int)vector[0].size();

  auto input = new Cell[width * height];
  for (unsigned int y = 0; y < height; ++y)
    for (unsigned int x = 0; x < width; ++x)
      input[y * width + x] = vector[y][x] ? ALIVE : DEAD;

  auto output = new Cell[width * height];
  if (a)
    return {input, output, width, height};
  else
    return {output, input, width, height};
}

std::vector<std::vector<Cell>> ungenerate(BoardMeta board) {
  const auto& output = board.output;
  const auto& width = board.width;
  const auto& height = board.height;

  std::vector<std::vector<Cell>> vector(height, std::vector<Cell>(width));
  for (unsigned int y = 0; y < height; ++y)
    for (unsigned int x = 0; x < width; ++x)
      vector[y][x] = output[y * width + x] == ALIVE ? 1 : 0;

  delete board.input;
  delete output;
  return vector;
}

void compare(BoardMeta a, BoardMeta b) {
  REQUIRE(ungenerate(a) == ungenerate(b));
}

TEST_CASE("nothing", "[nextBoard]") {
  auto input = generate({{false}});
  auto expected = generate({{false}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("death", "[nextBoard]") {
  auto input = generate({{true}});
  auto expected = generate({{false}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("block", "[nextBoard]") {
  auto input =
      generate({{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}});
  auto expected =
      generate({{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("block (vertical wrap)", "[nextBoard]") {
  auto input = generate({{0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}});
  auto expected = generate({{0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("block (horizontal wrap)", "[nextBoard]") {
  auto input = generate({{0, 0, 0}, {1, 0, 1}, {1, 0, 1}, {0, 0, 0}});
  auto expected = generate({{0, 0, 0}, {1, 0, 1}, {1, 0, 1}, {0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("block (corner wrap)", "[nextBoard]") {
  auto input = generate({{1, 0, 1}, {0, 0, 0}, {1, 0, 1}});
  auto expected = generate({{1, 0, 1}, {0, 0, 0}, {1, 0, 1}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("bee-hive", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("loaf", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 0, 1, 0},
       {0, 0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 0, 1, 0},
       {0, 0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("boat", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("tub", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("blinker 1", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 1, 1, 1, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("blinker 2", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 1, 1, 1, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("toad 1", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 1, 0},
       {0, 1, 1, 1, 0, 0},
       {0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 0, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 0, 0, 0},
       {0, 0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("toad 2", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 0, 1, 0, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 1, 0, 0, 1, 0},
       {0, 0, 1, 0, 0, 0},
       {0, 0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0},
       {0, 0, 1, 1, 1, 0},
       {0, 1, 1, 1, 0, 0},
       {0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("beacon 1", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 1, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("beacon 2", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 1, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("glider 1", "[nextBoard]") {
  auto input = generate(
      {{1, 0, 0, 0, 0},
       {0, 1, 1, 0, 0},
       {1, 1, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 1, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {1, 1, 1, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("glider 2", "[nextBoard]") {
  auto input = generate(
      {{0, 1, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {1, 1, 1, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0},
       {1, 0, 1, 0, 0},
       {0, 1, 1, 0, 0},
       {0, 1, 0, 0, 0},
       {0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("glider 3", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 1, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 1, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}

TEST_CASE("glider 4", "[nextBoard]") {
  auto input = generate(
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 1, 0},
       {0, 1, 0, 1, 0},
       {0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0}});
  auto expected = generate(
      {{0, 0, 0, 0, 0},
       {0, 0, 1, 0, 0},
       {0, 0, 0, 1, 1},
       {0, 0, 1, 1, 0},
       {0, 0, 0, 0, 0}}, false);

  nextBoard(input);
  compare(input, expected);
}
