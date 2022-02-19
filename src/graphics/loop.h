#pragma once

/* #include "../../build/_deps/imgui-sfml-src/imgui-SFML.h" */
/* #include "../../build/_deps/imgui-src/imgui.h" */
/* #include "../../build/_deps/sfml-src/include/SFML/Graphics.hpp" */
#include "../logic/board.h"
#include "SFML/Graphics.hpp"

struct Loop {
  Loop(const uint width, const uint height, const std::string title, const bool resizable);
  void run(const long maxComputations, uint threadCount, const uint startTargetRendersPerSecond);

  sf::RenderWindow window;
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Image image;
  sf::Uint32* pixels;
  Board board;
};
