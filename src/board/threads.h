#pragma once

#include <thread>

auto PROBABLY_OPTIMAL_THREAD_COUNT = std::max(std::thread::hardware_concurrency()*2, (unsigned int)1);
