#define CATCH_CONFIG_MAIN

#include <array>
#include <catch2/catch.hpp>
#include "../common/setBenchmarkBoard.h"
#include "../logic/next.h"
#include "../logic/padding.h"
#include "../logic/threads.h"

using BoardVector = std::vector<std::vector<bool>>;

void generate(Board& board, BoardVector vector) {
  const auto height = (unsigned int)vector.size();
  const auto width = (unsigned int)vector[0].size();

  board.setSize(width, height);
  for (unsigned int y = 1; y < height + 1; ++y)
    for (unsigned int x = 1; x < width + 1; ++x)
      board.output[y * board.rawWidth + x] = vector[y - 1][x - 1] ? ALIVE : DEAD;

  assignBoardPadding(board);
}

BoardVector ungenerate(Board& board) {
  const auto& output = board.output;
  const auto& width = board.width;
  const auto& height = board.height;

  std::vector<std::vector<bool>> vector(height, std::vector<bool>(width));
  for (unsigned int y = 1; y < height + 1; ++y)
    for (unsigned int x = 1; x < width + 1; ++x)
      vector[y - 1][x - 1] = output[y * board.rawWidth + x] == ALIVE ? 1 : 0;

  return vector;
}

void compare(BoardVector a, BoardVector b, uint generations = 1) {
  Board boardA = {1, 1};
  Board boardB = {1, 1};
  generate(boardA, a);
  generate(boardB, b);
  for (uint i = 0; i < generations; i++)
    nextBoard(boardA, PROBABLY_OPTIMAL_THREAD_COUNT, PROBABLY_OPTIMAL_JOB_COUNT);
  REQUIRE(ungenerate(boardA) == ungenerate(boardB));
}

TEST_CASE("nothing", "[nextBoard]") {
  compare(
      {{false}},

      {{false}});
}

TEST_CASE("death", "[nextBoard]") {
  compare(
      {{true}},

      {{false}});
}

TEST_CASE("block", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},

      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}});
}

TEST_CASE("block (3 generations)", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},

      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},

      3);
}

TEST_CASE("block (horizontal wrap, 3 generations)", "[nextBoard]") {
  compare(
      {{0, 0, 0}, {1, 0, 1}, {1, 0, 1}, {0, 0, 0}},

      {{0, 0, 0}, {1, 0, 1}, {1, 0, 1}, {0, 0, 0}},

      3);
}

TEST_CASE("block (vertical wrap, 3 generations)", "[nextBoard]") {
  compare(
      {{0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}},

      {{0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}},

      3);
}

TEST_CASE("block (corner wrap, 3 generations)", "[nextBoard]") {
  compare(
      {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}},

      {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}},

      3);
}

TEST_CASE("bee-hive (3 generations)", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 1, 0, 0, 1, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 1, 0, 0, 1, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 0, 0, 0}},

      3);
}

TEST_CASE("loaf (3 generations)", "[nextBoard]") {
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
       {0, 0, 0, 0, 0, 0}},

      3);
}

TEST_CASE("boat (3 generations)", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {0, 1, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {0, 1, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}},

      3);
}

TEST_CASE("tub (3 generations)", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}},

      3);
}

TEST_CASE("blinker 1 (3 generations)", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

      3);
}

TEST_CASE("blinker 2 (3 generations)", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}},

      3);
}

TEST_CASE("blinker 1 (vertical wrap, 5 generations)", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 0, 0, 1}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

      5);
}

TEST_CASE("blinker 1 (corner wrap, 5 generations)", "[nextBoard]") {
  compare(
      {{1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0, 0, 0, 0}},

      {{1, 1, 0, 0, 1}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

      5);
}

TEST_CASE("toad 1 (3 generations)", "[nextBoard]") {
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
       {0, 0, 0, 0, 0, 0}},

      3);
}

TEST_CASE("toad 2 (3 generations)", "[nextBoard]") {
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
       {0, 0, 0, 0, 0, 0}},

      3);
}

TEST_CASE("beacon 1 (3 generations)", "[nextBoard]") {
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
       {0, 0, 0, 0, 0, 0}},

      3);
}

TEST_CASE("beacon 2 (3 generations)", "[nextBoard]") {
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
       {0, 0, 0, 0, 0, 0}},

      3);
}

TEST_CASE("glider 1", "[nextBoard]") {
  compare(
      {{1, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {1, 1, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

      {{0, 1, 0, 0, 0}, {0, 0, 1, 0, 0}, {1, 1, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}});
}

TEST_CASE("glider 2", "[nextBoard]") {
  compare(
      {{0, 1, 0, 0, 0}, {0, 0, 1, 0, 0}, {1, 1, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0}, {1, 0, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 1, 0, 0, 0}, {0, 0, 0, 0, 0}});
}

TEST_CASE("glider 3", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0}, {0, 1, 0, 1, 0}, {0, 0, 1, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0}, {0, 0, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 0, 0}});
}

TEST_CASE("glider 4", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0}, {0, 0, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 1, 1}, {0, 0, 1, 1, 0}, {0, 0, 0, 0, 0}});
}

TEST_CASE("glider (2 generations)", "[nextBoard]") {
  compare(
      {{1, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {1, 1, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

      {{0, 0, 0, 0, 0}, {1, 0, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 1, 0, 0, 0}, {0, 0, 0, 0, 0}},

      2);
}

TEST_CASE("gliders wide horizontal wrap collision (long board)", "[nextBoard]") {
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

TEST_CASE("glider tall vertical wrap collision (tall board)", "[nextBoard]") {
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

TEST_CASE("medium glider (25 generations)", "[nextBoard]") {
  compare(
      {{0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0}},

      {{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}},

      25);
}

TEST_CASE("benchmark 2000 generations", "[nextBoard]") {
  Board test(2560, 1440);
  setBenchmarkBoard(test);

  // Expected result created with the following code
  /* Board board(2560, 1440); */
  /* setBenchmarkBoard(board); */

  /* for (uint i = 0; i < 2000; i++) { */
  /*   nextBoard(board, PROBABLY_OPTIMAL_THREAD_COUNT, PROBABLY_OPTIMAL_JOB_COUNT); */
  /* } */

  /* std::ofstream outfile; */
  /* outfile.open("src/test/expected.txt"); */
  /* for (uint i = 0; i < board.rawSize; i++) { */
  /*   outfile << (int)board.output[i]; */
  /* } */
  /* outfile.close(); */

  // Load expected benchmark result
  Board expected(2560, 1440);
  std::ifstream expectedFile("src/test/expected.txt");
  if (expectedFile.is_open()) {
    std::string line;
    getline(expectedFile, line);
    for (uint i = 0; i < expected.rawSize; i++) {
      if (line[i] == '1')
        expected.output[i] = 1;
      else if (line[i] == '0')
        expected.output[i] = 0;
      else
        throw std::runtime_error("not good man!");
    }
    expectedFile.close();
  } else {
    throw std::runtime_error("gotta go!");
  }

  // Run common benchmark scenario
  for (uint i = 0; i < 2000; i++)
    nextBoard(test, PROBABLY_OPTIMAL_THREAD_COUNT, PROBABLY_OPTIMAL_JOB_COUNT);

  // Assert that output matches expected results
  for (uint i = 0; i < test.rawSize; i++)
    if (test.output[i] != expected.output[i])
      throw std::runtime_error("2000 generations of the benchmark board, mismatched");
}
