#pragma once

#include "../logic/board.h"
#include "SFML/Graphics.hpp"

struct Loop {
  Loop(const uint width, const uint height, const std::string title, const bool resizable);
  ~Loop();
  void run(const ulong maxComputations, uint threadCount, const ulong renderMinimumMicroseconds);

  sf::RenderWindow window;
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Image image;
  sf::Uint32* pixels;
  Board board;
};
