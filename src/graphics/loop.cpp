#include <future>
#include <iostream>
#include <thread>
#include "../common/lock.h"
#include "../common/setBenchmarkBoard.h"
#include "../logic/next.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "loop.h"
#include "render.h"
#include "renderImguiMenu.h"
#include "time.h"

using namespace std::chrono;

Loop::Loop(const uint width, const uint height, const std::string title, const bool resizable)
    : window(sf::RenderWindow(sf::VideoMode(width, height), title, resizable ? sf::Style::Resize : sf::Style::None)),
      pixels(new sf::Uint32[(width + PADDING) * (height + PADDING)]),
      board(Board(width, height)) {
  // Init board
  setBenchmarkBoard(board);

  // Init graphics
  ImGui::SFML::Init(window);
  image.create(width + PADDING, height + PADDING, reinterpret_cast<sf::Uint8*>(pixels));
  texture.loadFromImage(image);
  sprite.setTexture(texture, true);
  sprite.setPosition(-1, -1);
}

Loop::~Loop() {
  delete[] pixels;
}

bool isExitEvent(const sf::Event& event) {
  return event.type == sf::Event::Closed ||
         (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape);
}

bool isResizeEvent(const sf::Event& event) {
  return event.type == sf::Event::Resized;
}

bool isResetEvent(const sf::Event& event) {
  return event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter;
}

bool isDrawEvent(const sf::Event& event) {
  return event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void drawToBoard(const sf::Event& event, Board& board) {
  constexpr int BRUSH_RADIUS = 30;
  for (int x = event.mouseMove.x - BRUSH_RADIUS; x < event.mouseMove.x + BRUSH_RADIUS; x++) {
    for (int y = event.mouseMove.y - BRUSH_RADIUS; y < event.mouseMove.y + BRUSH_RADIUS; y++) {
      board.output
          [(uint)std::clamp(y, 1, (int)board.height + 1) * (board.width + 2) +
           (uint)std::clamp(x, 1, (int)board.width + 1) + 1] = ALIVE;
    }
  }
}

void resizeBoard(const sf::Event& event, Board& board, sf::RenderWindow& window, sf::Uint32*& pixels) {
  window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
  delete[] pixels;
  pixels = new sf::Uint32[(event.size.width + 2) * (event.size.height + 2)];
  board.setSize(event.size.width, event.size.height);
  setBenchmarkBoard(board);
}

void handleEvent(const sf::Event& event, sf::RenderWindow& window, Board& board, sf::Uint32*& pixels) {
  if (isExitEvent(event)) {
    window.close();
  } else if (isResizeEvent(event)) {
    auto _ = LockForScope(board.lock);
    resizeBoard(event, board, window, pixels);
  } else if (isDrawEvent(event)) {
    auto _ = LockForScope(board.lock);
    drawToBoard(event, board);
  } else if (isResetEvent(event)) {
    auto _ = LockForScope(board.lock);
    setBenchmarkBoard(board);
  }
}

void Loop::run(const long maxComputations, uint threadCount, const uint startTargetRendersPerSecond) {
  auto& board = this->board;
  auto& window = this->window;

  // Statistics
  long totalComputations = 0;
  long totalRenders = 0;
  long computationsSinceLastGuiDraw = 0;

  // Configuration
  uint targetRendersPerSecond = startTargetRendersPerSecond;
  bool running = true;

  // Computation loop
  std::thread nextBoardThread([&]() {
    while (running && totalComputations < maxComputations) {
      board.lock.pauseIfRequested();

      nextBoard(board, threadCount);
      totalComputations++;
      computationsSinceLastGuiDraw++;
    }
  });

  // Render loop
  sf::Clock renderDeltaClock;
  while (window.isOpen() && totalComputations < maxComputations) {
    const auto renderDelta = renderDeltaClock.restart();
    auto renderTimer = start();

    ImGui::SFML::Update(window, renderDelta);

    drawBoard(board, window, sprite, texture, image, pixels);
    renderImguiMenu(
        board, window, renderDelta, totalComputations, computationsSinceLastGuiDraw, targetRendersPerSecond,
        threadCount);
    computationsSinceLastGuiDraw = 0;
    ImGui::SFML::Render(window);
    window.display();

    sf::Event event;
    while (window.pollEvent(event)) {
      handleEvent(event, window, board, pixels);

      ImGui::SFML::ProcessEvent(event);
      if (isExitEvent(event))
        ImGui::SFML::Shutdown();
    }

    // Limit render rate
    stopAndDelay(renderTimer, 1000000 / targetRendersPerSecond);
    totalRenders++;
  }

  // Close computation thread
  running = false;
  nextBoardThread.join();
}
