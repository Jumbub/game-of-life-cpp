#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <cstring>
#include <vector>
#include "../common/setBenchmarkBoard.h"
#include "../logic/board.h"
#include "renderBoard.h"

constexpr uint64_t BLACK = 0xff000000;
constexpr uint64_t WHITE_TRANSPARENT = 0x00ffffff;

void renderBoard(Board& board, sf::RenderWindow& window, sf::Sprite& sprite, sf::Texture& texture, sf::Uint32* pixels) {
  const uint limitI = board.rawWidth * board.rawHeight - board.rawWidth;
  for (uint i = board.rawWidth; i < limitI; i++) {
    pixels[i] = BLACK + WHITE_TRANSPARENT * board.output[i];
  }

  texture.update(reinterpret_cast<sf::Uint8*>(pixels));
  sprite.setTexture(texture, false);
  window.draw(sprite);
}
