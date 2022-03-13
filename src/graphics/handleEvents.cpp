#include <chrono>
#include "../common/setBenchmarkBoard.h"
#include "handleEvents.h"

bool isExitEvent(const sf::Event& event) {
  return event.type == sf::Event::Closed ||
         (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape);
}

bool isResizeEvent(const sf::Event& event) {
  return event.type == sf::Event::Resized;
}

bool isResetEvent(const sf::Event& event) {
  return event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter;
}

bool isScreenshotEvent(const sf::Event& event) {
  return event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space;
}

bool isDrawEvent(const sf::Event& event) {
  return event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void drawToBoard(const sf::Event& event, Board& board) {
  constexpr int BRUSH_RADIUS = 30;
  for (int x = event.mouseMove.x - BRUSH_RADIUS; x < event.mouseMove.x + BRUSH_RADIUS; x++) {
    for (int y = event.mouseMove.y - BRUSH_RADIUS; y < event.mouseMove.y + BRUSH_RADIUS; y++) {
      board.output
          [(uint)std::clamp(y, (int)PADDING / 2, (int)board.height + (int)PADDING / 2) * board.rawWidth +
           (uint)std::clamp(x, (int)PADDING / 2, (int)board.width + (int)PADDING / 2) + 1] = ALIVE;
    }
  }
}

void resizeBoard(
    const sf::Event& event,
    Board& board,
    sf::RenderWindow& window,
    sf::Uint32*& pixels,
    sf::Texture& texture,
    sf::Sprite& sprite) {
  window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));

  if (event.size.height == board.height && event.size.width == board.width)
    return;

  delete[] pixels;
  pixels = new sf::Uint32[(event.size.width + PADDING) * (event.size.height + PADDING)];
  board.setSize(event.size.width, event.size.height);
  texture.create(event.size.width + PADDING, event.size.height + PADDING);
  sprite.setTexture(texture, true);
  setBenchmarkBoard(board);
}

void screenshot(sf::RenderWindow& window) {
  sf::Vector2u windowSize = window.getSize();
  sf::Texture texture;
  texture.create(windowSize.x, windowSize.y);
  texture.update(window);
  sf::Image screenshot = texture.copyToImage();
  screenshot.saveToFile(
      std::string("screenshot") + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) +
      std::string(".png"));
}
