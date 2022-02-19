#pragma once

#include "../logic/board.h"
#include "SFML/Graphics.hpp"

void renderImguiMenu(
    Board& board,
    sf::RenderWindow& window,
    const sf::Time& renderDelta,
    const ulong& computedGenerations,
    uint& threadCount);
