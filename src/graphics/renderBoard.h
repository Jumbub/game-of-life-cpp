#pragma once

#include "../logic/board.h"
#include "SFML/Graphics.hpp"

void renderBoard(Board& board, sf::RenderWindow& window, sf::Sprite& sprite, sf::Texture& texture, sf::Uint32* pixels);
