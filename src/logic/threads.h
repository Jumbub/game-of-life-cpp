#pragma once

#include <thread>

// The number of threads that was optimal in the benchmarks
auto MAYBE_OPTIMAL_THREAD_COUNT = std::max(std::thread::hardware_concurrency() * 3, (unsigned int)1);
