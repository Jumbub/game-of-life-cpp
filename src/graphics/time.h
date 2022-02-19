#pragma once

#include <chrono>

using TimeMeta = std::chrono::time_point<std::chrono::steady_clock>;

TimeMeta start();
void stopAndDelay(TimeMeta startTime, long minMacros);
