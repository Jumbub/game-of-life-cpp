#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <cstring>
#include <vector>
#include "../common/setBenchmarkBoard.h"
#include "../logic/board.h"
#include "renderBoard.h"

constexpr uint32_t SKIP_DEAD = 0x10dd0000;
constexpr uint32_t SKIP_ALIVE = 0xff888888 - SKIP_DEAD;

constexpr uint32_t NO_SKIP_DEAD = 0x10dd0000;
constexpr uint32_t NO_SKIP_ALIVE = 0xffffffff - NO_SKIP_DEAD;

constexpr bool SHOW_SKIPS = 0;

void renderBoard(
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

  const uint limit = width * height - width;
  for (uint i = width; i < limit; i++) {
    if (SHOW_SKIPS) {
      pixels[i] = board.inSkip[i] ? SKIP_DEAD + SKIP_ALIVE * output[i] : NO_SKIP_DEAD + NO_SKIP_ALIVE * output[i];
    } else {
      pixels[i] = NO_SKIP_DEAD + NO_SKIP_ALIVE * output[i];
    }
  }

  image.create(width, height, reinterpret_cast<sf::Uint8*>(pixels));
  texture.update(image);
  sprite.setTexture(texture, true);
  window.draw(sprite);
}
