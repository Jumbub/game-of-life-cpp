#include "../../build/_deps/imgui-src/imgui.h"
#include "../../build/_deps/sfml-src/include/SFML/Graphics.hpp"
#include "../common/setBenchmarkBoard.h"
#include "../logic/board.h"
#include "../logic/threads.h"
#include "gui.h"

// ImGui decleration
void drawGui(
    Board& board,
    const sf::RenderWindow& window,
    const sf::Time& renderDelta,
    const uint& totalComputations,
    const uint& computationsSinceLastRender,
    uint& targetRendersPerSecond,
    uint& threadCount) {
  ImGui::Begin("Configuration");

  // Basic
  ImGui::Text("Total computations: %d", totalComputations);
  ImGui::Text("Board size: %dx%d", board.width, board.height);
  ImGui::Text("Window size: %dx%d", window.getSize().x, window.getSize().y);

  // Render
  ImGui::Text("Renders/second: %.2f", 1.0 / renderDelta.asSeconds());
  int rps = (int)targetRendersPerSecond;
  ImGui::SliderInt("Target renders/second", &rps, 1, 300);
  if ((uint)rps != targetRendersPerSecond) {
    auto scope = LockForScope(board.lock);
    targetRendersPerSecond = (uint)rps;
  }

  // Compute
  ImGui::Text("Computations/second: %.2f", (float)computationsSinceLastRender / renderDelta.asSeconds());
  int tpb = (int)threadCount;
  ImGui::SliderInt("Threads/computation", &tpb, 1, (int)PROBABLY_OPTIMAL_THREAD_COUNT * 4);
  if ((uint)tpb != threadCount) {
    auto scope = LockForScope(board.lock);
    threadCount = (uint)tpb;
  }

  // Restart
  if (ImGui::Button("Restart")) {
    auto scope = LockForScope(board.lock);
    setBenchmarkBoard(board);
  }

  ImGui::End();
}
