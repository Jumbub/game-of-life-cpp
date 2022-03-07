#include <future>
#include <iostream>
#include <thread>
#include "../common/lock.h"
#include "../common/setBenchmarkBoard.h"
#include "../logic/next.h"
#include "handleEvents.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "loop.h"
#include "renderBoard.h"
#include "renderImguiMenu.h"

using namespace std::chrono;

Loop::Loop(const uint width, const uint height, const std::string title, const bool resizable)
    : window(sf::RenderWindow(sf::VideoMode(width, height), title, resizable ? sf::Style::Resize : sf::Style::None)),
      pixels(new sf::Uint32[(width + PADDING) * (height + PADDING)]),
      board(Board(width, height)) {
  setBenchmarkBoard(board);

  ImGui::SFML::Init(window);
  texture.create(width + PADDING, height + PADDING);
  sprite.setTexture(texture, true);
  sprite.setPosition((int)-PADDING / 2, (int)-PADDING / 2);
}

Loop::~Loop() {
  delete[] pixels;
}

void Loop::run(ulong maxGenerations, uint threadCount, uint jobCount, ulong renderMinimumMicroseconds) {
  ulong computedGenerations = 0;
  auto nextBoardThread = startNextBoardLoopThread(maxGenerations, threadCount, jobCount, board, computedGenerations);

  sf::Clock clock;
  while (window.isOpen() && computedGenerations < maxGenerations) {
    const auto delta = clock.restart();

    ImGui::SFML::Update(window, delta);

    renderBoard(board, window, sprite, texture, pixels);
    renderImguiMenu(board, window, delta, computedGenerations, threadCount, jobCount, renderMinimumMicroseconds);

    ImGui::SFML::Render(window);

    window.display();

    sf::Event event;
    std::vector<sf::Event> mouseEvents;
    while (window.pollEvent(event)) {
      if (isExitEvent(event)) {
        window.close();
      } else if (isResizeEvent(event)) {
        auto _ = LockForScope(board.lock);
        resizeBoard(event, board, window, pixels, texture, sprite);
      } else if (isDrawEvent(event)) {
        mouseEvents.push_back(event);
      } else if (isResetEvent(event)) {
        auto _ = LockForScope(board.lock);
        setBenchmarkBoard(board);
      }

      ImGui::SFML::ProcessEvent(event);
      if (isExitEvent(event))
        ImGui::SFML::Shutdown();
    }
    if (!mouseEvents.empty()) {
      auto _ = LockForScope(board.lock);
      for (auto event : mouseEvents) {
        drawToBoard(event, board);
      }
    }

    std::this_thread::sleep_for(
        std::chrono::microseconds(renderMinimumMicroseconds - (ulong)clock.getElapsedTime().asMicroseconds()));
  }

  maxGenerations = 0;  // Notify the next board thread to end execution
  nextBoardThread.join();
}
