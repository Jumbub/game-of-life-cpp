#pragma once

#include "../logic/board.h"
#include "SFML/Graphics.hpp"

void drawBoard(
    Board& board,
    sf::RenderWindow& window,
    sf::Sprite& sprite,
    sf::Texture& texture,
    sf::Image& image,
    sf::Uint32* pixels);
