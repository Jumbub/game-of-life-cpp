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

bool isDrawEvent(const sf::Event& event) {
  return event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void drawToBoard(const sf::Event& event, Board& board) {
  constexpr int BRUSH_RADIUS = 30;
  for (int x = event.mouseMove.x - BRUSH_RADIUS; x < event.mouseMove.x + BRUSH_RADIUS; x++) {
    for (int y = event.mouseMove.y - BRUSH_RADIUS; y < event.mouseMove.y + BRUSH_RADIUS; y++) {
      board.output
          [(uint)std::clamp(y, 1, (int)board.height + 1) * (board.width + 2) +
           (uint)std::clamp(x, 1, (int)board.width + 1) + 1] = ALIVE;
    }
  }
}

void resizeBoard(const sf::Event& event, Board& board, sf::RenderWindow& window, sf::Uint32*& pixels) {
  window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
  delete[] pixels;
  pixels = new sf::Uint32[(event.size.width + 2) * (event.size.height + 2)];
  board.setSize(event.size.width, event.size.height);
  setBenchmarkBoard(board);
}
