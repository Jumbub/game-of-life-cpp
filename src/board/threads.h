#pragma once

#include <thread>

auto PROBABLY_OPTIMAL_THREAD_COUNT = std::max(std::thread::hardware_concurrency(), (unsigned int)1);
auto THREAD_COUNT = PROBABLY_OPTIMAL_THREAD_COUNT;
