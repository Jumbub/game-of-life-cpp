#pragma once

#include <thread>

// The number of threads that was optimal in the benchmarks
const uint PROBABLY_OPTIMAL_THREAD_COUNT = std::max(std::thread::hardware_concurrency(), (unsigned int)1);
const uint PROBABLY_OPTIMAL_JOB_COUNT = 4*PROBABLY_OPTIMAL_THREAD_COUNT;
