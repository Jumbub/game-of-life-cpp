#include "logic.cpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>

int main() {
  sf::RenderWindow window(sf::VideoMode(200, 200), "game-of-speed");
  auto [width, height] = window.getSize();

  auto board = randomBoard(width, height);

  sf::Image image;
  image.create(width, height);

  sf::Texture texture;
  texture.loadFromImage(image);

  sf::Sprite sprite;
  sprite.setTexture(texture);

  while (window.isOpen()) {
    // Compute next board
    board = nextBoard(board);

    // Render board
    for (int y = 0; y < height; y++)
      for (int x = 0; x < width; x++)
        if (board[y][x])
          image.setPixel(x, y, sf::Color::White);
        else
          image.setPixel(x, y, sf::Color::Black);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    window.draw(sprite);
    window.display();

    // Window events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          event.type == sf::Event::KeyPressed)
        window.close();
    }
  }

  return 0;
}
