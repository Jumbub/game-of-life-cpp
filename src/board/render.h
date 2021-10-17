#pragma once

#include <cstring>
#include <vector>
#include "../../build/_deps/imgui-sfml-src/imgui-SFML.h"
#include "../../build/_deps/imgui-src/imgui.h"
#include "../../build/_deps/sfml-src/include/SFML/Graphics.hpp"
#include "board.h"
#include "generate.h"

void render(
    BoardMeta& board,
    sf::RenderWindow& window,
    sf::Sprite& sprite,
    sf::Texture& texture,
    sf::Image& image,
    sf::Uint32* pixels) {
  const auto& output = board.output;

  /* auto pixels64 = reinterpret_cast<sf::Uint64*>(pixels); */
  for (unsigned int y = 1; y < board.height + 1; y++) {
    for (unsigned int x = 1; x < board.width + 1; x++) {
      if (output[y * (board.width + 2) + x] == ALIVE) {
        pixels[(y - 1) * board.width + (x - 1)] = 0xffffffff;
      } else {
        pixels[(y - 1) * board.width + (x - 1)] = 0xff000000;
      }
    }
  }

  image.create(board.width, board.height, reinterpret_cast<sf::Uint8*>(pixels));
  texture.update(image);
  sprite.setTexture(texture, true);
  window.draw(sprite);
}
