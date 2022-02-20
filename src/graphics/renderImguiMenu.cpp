#include "../../build/_deps/imgui-src/imgui.h"
#include "../../build/_deps/sfml-src/include/SFML/Graphics.hpp"
#include "../common/setBenchmarkBoard.h"
#include "../logic/board.h"
#include "../logic/threads.h"
#include "renderImguiMenu.h"

void renderImguiMenu(
    Board& board,
    sf::RenderWindow& window,
    const sf::Time& renderDelta,
    const ulong& computedGenerations,
    uint& threadCount,
    ulong& renderMinimumMicroseconds) {
  ImGui::Begin("Configuration");

  // Info
  ImGui::Text("Computed generations: %d", (int)computedGenerations);
  ImGui::Text("Board size: %dx%d", board.width, board.height);
  ImGui::Text("Window size: %dx%d", window.getSize().x, window.getSize().y);

  // Renders per second
  ImGui::Text("Renders/second: %.2f", 1.0 / renderDelta.asSeconds());
  static int rendersPerSecond = (float)1 / ((float)renderMinimumMicroseconds / (float)1000000);
  static int lastRendersPerSecond = rendersPerSecond;  // This 2 step process is necessary for the same reason above
  ImGui::SliderInt("Target renders/second", &rendersPerSecond, 1, 300);
  if (rendersPerSecond != lastRendersPerSecond) {
    auto scope = LockForScope(board.lock);
    renderMinimumMicroseconds = 1000000 / (uint)rendersPerSecond;
  }

  // Threads per frame
  static ulong lastComputedGenerations = 0;
  ImGui::Text(
      "Computations/second: %.2f", (float)(computedGenerations - lastComputedGenerations) / renderDelta.asSeconds());
  lastComputedGenerations = computedGenerations;
  int tpb = (int)threadCount;
  ImGui::SliderInt("Threads/computation", &tpb, 1, (int)PROBABLY_OPTIMAL_THREAD_COUNT * 4);
  if ((uint)tpb != threadCount) {
    auto scope = LockForScope(board.lock);
    threadCount = (uint)tpb;
  }

  // Reset action
  if (ImGui::Button("Restart")) {
    auto scope = LockForScope(board.lock);
    setBenchmarkBoard(board);
  }

  ImGui::End();
}
