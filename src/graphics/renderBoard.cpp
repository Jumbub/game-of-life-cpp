#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <cstring>
#include <vector>
#include "../common/setBenchmarkBoard.h"
#include "../logic/board.h"
#include "renderBoard.h"

constexpr uint32_t COLOR_DEAD = 0x10dd0000;
constexpr uint32_t COLOR_ALIVE = 0xffffffff - COLOR_DEAD;

void renderBoard(Board& board, sf::RenderWindow& window, sf::Sprite& sprite, sf::Texture& texture, sf::Uint32* pixels) {
  const uint limitI = board.rawWidth * board.rawHeight - board.rawWidth;
  for (uint i = board.rawWidth; i < limitI; i++) {
    pixels[i] = COLOR_DEAD + COLOR_ALIVE * board.output[i];
  }

  texture.update(reinterpret_cast<sf::Uint8*>(pixels));
  sprite.setTexture(texture, false);
  window.draw(sprite);
}
