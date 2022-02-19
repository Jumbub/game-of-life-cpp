#pragma once

#include "../logic/board.h"
#include "SFML/Graphics.hpp"

// ImGui decleration
void drawGui(
    Board& board,
    const sf::RenderWindow& window,
    const sf::Time& renderDelta,
    const uint& totalComputations,
    const uint& computationsSinceLastRender,
    uint& targetRendersPerSecond,
    uint& threadCount);
