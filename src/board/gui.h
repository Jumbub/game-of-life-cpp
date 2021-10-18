#pragma once

#include <vector>
#include <mutex>
#include <numeric>
#include "../../build/_deps/imgui-sfml-src/imgui-SFML.h"
#include "../../build/_deps/imgui-src/imgui.h"
#include "../../build/_deps/sfml-src/include/SFML/Graphics.hpp"
#include "board.h"

std::vector<float> computationsList(100);
std::vector<float> rendersList(100);

void gui(BoardMeta& board, sf::RenderWindow& window, sf::Time renderDelta, uint computations, uint renders) {
  static uint lastComputations = 0;
  static uint lastRenders = 0;

  while (computationsList.size() > 99)
    computationsList.pop_back();
  computationsList.insert(computationsList.begin(), (float)(computations - lastComputations)/renderDelta.asSeconds());

  while (rendersList.size() > 99)
    rendersList.pop_back();
  rendersList.insert(rendersList.begin(), (float)(renders - lastRenders)/renderDelta.asSeconds());

  ImGui::Begin("Configuration");
  ImGui::Text("Generation: %d", board.generation);
  ImGui::Text("Computations: %d", computations);
  ImGui::Text("Renders: %d", renders);
  ImGui::Text("Board size: %dx%d", board.width, board.height);
  ImGui::Text("Window size: %dx%d", window.getSize().x, window.getSize().y);

  ImGui::Text("Renders/second: %.2f", 1.0/renderDelta.asSeconds());
  ImGui::PlotLines("Renders/second", (float*)&rendersList[0], rendersList.size());
  int rps = (int)board.rendersPerSecond;
  ImGui::SliderInt("Target renders/second", &rps, 1, 300);
  if ((uint)rps != board.rendersPerSecond) {
    std::scoped_lock gaurd(board.mutex);
    board.rendersPerSecond = (uint)rps;
  }

  ImGui::Text("Boards/second: %.2f", (float)(computations - lastComputations)/renderDelta.asSeconds());
  ImGui::PlotLines("Boards/second", (float*)&computationsList[0], computationsList.size());

  int tpb = (int)board.threadsPerBoard;
  ImGui::SliderInt("Threads/board", &tpb, 1, (int)PROBABLY_OPTIMAL_THREAD_COUNT * 4);
  if ((uint)tpb != board.threadsPerBoard) {
    std::scoped_lock gaurd(board.mutex);
    board.threadsPerBoard = (uint)tpb;
  }

  lastComputations = computations;
  lastRenders = renders;

  ImGui::End();
}
