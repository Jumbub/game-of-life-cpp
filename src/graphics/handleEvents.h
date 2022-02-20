#pragma once

#include <SFML/Graphics.hpp>
#include "../logic/board.h"

bool isExitEvent(const sf::Event& event);
bool isResizeEvent(const sf::Event& event);
bool isResetEvent(const sf::Event& event);
bool isDrawEvent(const sf::Event& event);

void drawToBoard(const sf::Event& event, Board& board);
void resizeBoard(
    const sf::Event& event,
    Board& board,
    sf::RenderWindow& window,
    sf::Uint32*& pixels,
    sf::Texture& texture);
