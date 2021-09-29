#define CATCH_CONFIG_MAIN

#include "../board/next.h"
#include <catch2/catch.hpp>

TEST_CASE("nothing", "[nextBoard]") {
  Board input = {{false}};
  Board expected = {{false}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("death", "[nextBoard]") {
  Board input = {{true}};
  Board expected = {{false}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("block", "[nextBoard]") {
  Board input = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("block (vertical wrap)", "[nextBoard]") {
  Board input = {{0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}};
  Board expected = {{0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("block (horizontal wrap)", "[nextBoard]") {
  Board input = {{0, 0, 0}, {1, 0, 1}, {1, 0, 1}, {0, 0, 0}};
  Board expected = {{0, 0, 0}, {1, 0, 1}, {1, 0, 1}, {0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("block (corner wrap)", "[nextBoard]") {
  Board input = {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}};
  Board expected = {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("bee-hive", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0, 0},
                 {0, 0, 1, 1, 0, 0},
                 {0, 1, 0, 0, 1, 0},
                 {0, 0, 1, 1, 0, 0},
                 {0, 0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0, 0},
                    {0, 0, 1, 1, 0, 0},
                    {0, 1, 0, 0, 1, 0},
                    {0, 0, 1, 1, 0, 0},
                    {0, 0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("loaf", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 1, 0, 0, 1, 0},
                 {0, 0, 1, 0, 1, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 1, 0, 0, 1, 0},
                    {0, 0, 1, 0, 1, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("boat", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0},
                 {0, 1, 1, 0, 0},
                 {0, 1, 0, 1, 0},
                 {0, 0, 1, 0, 0},
                 {0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0},
                    {0, 1, 1, 0, 0},
                    {0, 1, 0, 1, 0},
                    {0, 0, 1, 0, 0},
                    {0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("tub", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0},
                 {0, 0, 1, 0, 0},
                 {0, 1, 0, 1, 0},
                 {0, 0, 1, 0, 0},
                 {0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0},
                    {0, 0, 1, 0, 0},
                    {0, 1, 0, 1, 0},
                    {0, 0, 1, 0, 0},
                    {0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("blinker 1", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0},
                 {0, 0, 1, 0, 0},
                 {0, 0, 1, 0, 0},
                 {0, 0, 1, 0, 0},
                 {0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0},
                    {0, 1, 1, 1, 0},
                    {0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("blinker 2", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0},
                 {0, 1, 1, 1, 0},
                 {0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0},
                    {0, 0, 1, 0, 0},
                    {0, 0, 1, 0, 0},
                    {0, 0, 1, 0, 0},
                    {0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("toad 1", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 1, 1, 1, 0},
                 {0, 1, 1, 1, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 1, 0, 0, 1, 0},
                    {0, 1, 0, 0, 1, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("toad 2", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 1, 0, 0, 1, 0},
                 {0, 1, 0, 0, 1, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 1, 1, 1, 0},
                    {0, 1, 1, 1, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("beacon 1", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 1, 0, 0, 0, 0},
                 {0, 0, 0, 0, 1, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 1, 1, 0, 0, 0},
                    {0, 0, 0, 1, 1, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("beacon 2", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 1, 1, 0, 0, 0},
                 {0, 0, 0, 1, 1, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 1, 0, 0, 0, 0},
                    {0, 0, 0, 0, 1, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("glider 1", "[nextBoard]") {
  Board input = {{1, 0, 0, 0, 0},
                 {0, 1, 1, 0, 0},
                 {1, 1, 0, 0, 0},
                 {0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0}};
  Board expected = {{0, 1, 0, 0, 0},
                    {0, 0, 1, 0, 0},
                    {1, 1, 1, 0, 0},
                    {0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("glider 2", "[nextBoard]") {
  Board input = {{0, 1, 0, 0, 0},
                 {0, 0, 1, 0, 0},
                 {1, 1, 1, 0, 0},
                 {0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0},
                    {1, 0, 1, 0, 0},
                    {0, 1, 1, 0, 0},
                    {0, 1, 0, 0, 0},
                    {0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("glider 3", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0},
                 {0, 1, 0, 1, 0},
                 {0, 0, 1, 1, 0},
                 {0, 0, 1, 0, 0},
                 {0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0},
                    {0, 0, 0, 1, 0},
                    {0, 1, 0, 1, 0},
                    {0, 0, 1, 1, 0},
                    {0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}

TEST_CASE("glider 4", "[nextBoard]") {
  Board input = {{0, 0, 0, 0, 0},
                 {0, 0, 0, 1, 0},
                 {0, 1, 0, 1, 0},
                 {0, 0, 1, 1, 0},
                 {0, 0, 0, 0, 0}};
  Board expected = {{0, 0, 0, 0, 0},
                    {0, 0, 1, 0, 0},
                    {0, 0, 0, 1, 1},
                    {0, 0, 1, 1, 0},
                    {0, 0, 0, 0, 0}};

  REQUIRE(nextBoard(input) == expected);
}
