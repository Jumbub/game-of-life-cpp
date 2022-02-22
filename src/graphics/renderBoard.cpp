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

void renderBoard(
    Board& board,
    [[maybe_unused]] sf::RenderWindow& window,
    sf::Sprite& sprite,
    sf::Texture& texture,
    sf::Uint32* pixels) {
  const auto& output = board.output;

  // Generate the pixel texture data from the board output

  const uint limit = board.rawWidth * board.rawHeight - board.rawWidth;
  for (uint i = board.rawWidth; i < limit; i++) {
    pixels[i] = COLOR_DEAD + COLOR_ALIVE * output[i];
  }

  texture.update(reinterpret_cast<sf::Uint8*>(pixels));
  sprite.setTexture(texture, false);
  window.draw(sprite);
}
