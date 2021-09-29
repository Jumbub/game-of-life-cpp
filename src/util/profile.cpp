#include <chrono>
#include <iostream>

using milliseconds = std::chrono::milliseconds;

using ProfileData = std::chrono::time_point<std::chrono::steady_clock>;
ProfileData startProfiling() { return std::chrono::steady_clock::now(); }
void stopProfiling(ProfileData startTime, std::string label) {
  std::cout << label << " time: "
            << std::chrono::duration_cast<milliseconds>(
                   std::chrono::steady_clock::now() - startTime)
                   .count()
            << " ms" << std::endl;
}
