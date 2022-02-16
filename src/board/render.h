#pragma once

#include <cstring>
#include <vector>
#include "../../build/_deps/imgui-sfml-src/imgui-SFML.h"
#include "../../build/_deps/imgui-src/imgui.h"
#include "../../build/_deps/sfml-src/include/SFML/Graphics.hpp"
#include "board.h"
#include "generate.h"

// I found this number by accident, it looks beautiful.
constexpr uint MAGIC_NUMBER = 267386880;
constexpr uint MAGIC_NUMBER_TO_MAX = UINT32_MAX - MAGIC_NUMBER;

void drawBoard(
    Board& board,
    sf::RenderWindow& window,
    sf::Sprite& sprite,
    sf::Texture& texture,
    sf::Image& image,
    sf::Uint32* pixels) {
  const auto& output = board.output;

  // Generate the pixel texture data from the board output
  uint width = board.width + 2;
  uint height = board.height + 2;
  for (unsigned int y = 0; y < height; y++) {
    for (unsigned int x = 0; x < width; x++) {
      const uint offset = y * width + x;
      pixels[offset] = output[offset][0] * MAGIC_NUMBER_TO_MAX + MAGIC_NUMBER;
    }
  }

  image.create(width, height, reinterpret_cast<sf::Uint8*>(pixels));
  texture.update(image);
  sprite.setTexture(texture, true);
  window.draw(sprite);
}
