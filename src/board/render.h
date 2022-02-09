#pragma once

#include <cstring>
#include <vector>
#include "../../build/_deps/imgui-sfml-src/imgui-SFML.h"
#include "../../build/_deps/imgui-src/imgui.h"
#include "../../build/_deps/sfml-src/include/SFML/Graphics.hpp"
#include "board.h"
#include "generate.h"

void drawBoard(
    Board& board,
    sf::RenderWindow& window,
    sf::Sprite& sprite,
    sf::Texture& texture,
    sf::Image& image,
    sf::Uint32* pixels) {
  const auto& output = board.output;

  // Generate the pixel texture data from the board output
  for (unsigned int y = 1; y < board.height + 1; y++) {
    for (unsigned int x = 1; x < board.width + 1; x++) {
      if (output[y * (board.width + 2) + x] == ALIVE) {
        pixels[(y - 1) * board.width + (x - 1)] = UINT32_MAX;
      } else {
        pixels[(y - 1) * board.width + (x - 1)] = 267386880;
      }
    }
  }

  image.create(board.width, board.height, reinterpret_cast<sf::Uint8*>(pixels));
  texture.update(image);
  sprite.setTexture(texture, true);
  window.draw(sprite);
}
