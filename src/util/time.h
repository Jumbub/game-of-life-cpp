#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std::chrono;

using TimeMeta = std::chrono::time_point<std::chrono::steady_clock>;

TimeMeta start() {
  return std::chrono::steady_clock::now();
}

void stopAndDelay(TimeMeta startTime, long minMacros) {
  auto micros = duration_cast<microseconds>(steady_clock::now() - startTime).count();
  auto sleepTime = std::max(minMacros - micros, (long)0);
  std::this_thread::sleep_for(microseconds(sleepTime));
}
