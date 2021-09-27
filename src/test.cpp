#define CATCH_CONFIG_MAIN

#include "logic.cpp"
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
