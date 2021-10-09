#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>

using ProfileData = std::chrono::time_point<std::chrono::steady_clock>;
ProfileData startProfiling() {
  return std::chrono::steady_clock::now();
}
void stopProfiling(ProfileData startTime, std::string label) {
  const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::steady_clock::now() - startTime)
                      .count();
  const auto mcs = std::chrono::duration_cast<std::chrono::microseconds>(
                       std::chrono::steady_clock::now() - startTime)
                       .count();
  const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                      std::chrono::steady_clock::now() - startTime)
                      .count();

  if (mcs < 5) {
    std::cout << std::setw(8) << ns << " nanos  | " << label << std::endl;
  } else if (ms < 5) {
    std::cout << std::setw(8) << mcs << " micros | " << label << std::endl;
  } else {
    std::cout << std::setw(8) << ms << " millis | " << label << std::endl;
  }
}
