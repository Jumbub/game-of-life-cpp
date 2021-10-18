#include <future>
#include <iostream>
#include <thread>
#include "../../build/_deps/imgui-sfml-src/imgui-SFML.h"
#include "../../build/_deps/imgui-src/imgui.h"
#include "../../build/_deps/sfml-src/include/SFML/Graphics.hpp"
#include "../board/generate.h"
#include "../board/next.h"
#include "../board/render.h"
#include "../board/gui.h"
#include "../util/time.h"

using namespace std::chrono;

struct LoopMeta {
  sf::RenderWindow* window = nullptr;
  sf::Texture* texture = new sf::Texture;
  sf::Sprite* sprite = new sf::Sprite;
  sf::Image* image = new sf::Image;
  sf::Uint32* pixels = new sf::Uint32[2560 * 1440];
  BoardMeta* board = nullptr;
};

LoopMeta setup(bool fullscreen = false) {
  LoopMeta loop;
  loop.board = new BoardMeta(2560, 1440);

  // Init board
  benchmarkBoard(*loop.board, (uint)loop.board->width, (uint)loop.board->height);

  // Init graphics
  loop.window = new sf::RenderWindow(sf::VideoMode(2560, 1440), "Game of Speed", fullscreen ? sf::Style::None : sf::Style::Resize);
  loop.window->setVerticalSyncEnabled(false);
  ImGui::SFML::Init(*loop.window);
  loop.image->create(2560, 1440, reinterpret_cast<sf::Uint8*>(loop.pixels));
  loop.texture->loadFromImage(*loop.image);
  loop.sprite->setTexture(*loop.texture, true);

  return loop;
}

void loop(LoopMeta loop, long maxComputations) {
  auto totalTimer = start();

  auto& board = *loop.board;
  auto& window = loop.window;

  long computations = 0;
  long renders = 0;

  // Computation loop
  std::thread nextBoardThread([&board, &window, &computations, &maxComputations]() {
    while (window->isOpen() && computations < maxComputations) {
      std::this_thread::sleep_for(std::chrono::nanoseconds(1));
      std::scoped_lock gaurd(board.mutex);
      nextBoard(board);
      computations++;
    }
  });

  sf::Clock deltaClock;

  // Render loop
  while (loop.window->isOpen() && computations < maxComputations) {
    auto renderTimer = start();
    sf::Event event;
    while (loop.window->pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
        std::scoped_lock gaurd(board.mutex);
        loop.window->close();
      } else if (event.type == sf::Event::Resized) {
        std::scoped_lock gaurd(board.mutex);
        window->setView(sf::View(sf::FloatRect(0,0,event.size.width, event.size.height)));
        delete[] loop.pixels;
        loop.pixels = new sf::Uint32[event.size.width * event.size.height];
        benchmarkBoard(board, event.size.width, event.size.height);
      } else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)) {
        std::scoped_lock gaurd(board.mutex);
        benchmarkBoard(board, board.width, board.height);
      } else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::J)) {
        std::scoped_lock gaurd(board.mutex);
        board.threadsPerBoard = std::max(board.threadsPerBoard - 1, (uint)1);
      } else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::K)) {
        std::scoped_lock gaurd(board.mutex);
        board.threadsPerBoard++;
      }
    }
    const auto deltaTime = deltaClock.restart();
    ImGui::SFML::Update(*loop.window, deltaTime);

    gui(board, *loop.window, deltaTime, computations, renders);

    render(board, *loop.window, *loop.sprite, *loop.texture, *loop.image, loop.pixels);
    renders++;
    ImGui::SFML::Render(*loop.window);
    window->display();

    stopAndDelay(renderTimer, 1000000 / board.rendersPerSecond);
  }

  nextBoardThread.join();

  stopAndFps(totalTimer, renders, computations);
}

void shutdown(LoopMeta loop) {
  ImGui::SFML::Shutdown();

  delete loop.image;
  /* delete loop.texture; */
  delete loop.sprite;
  delete loop.window;

  delete loop.board;
}
