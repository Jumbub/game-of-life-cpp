#pragma once

#include <chrono>
#include <iostream>

using ProfileData = std::chrono::time_point<std::chrono::steady_clock>;

ProfileData startProfiling();
void stopProfiling(ProfileData startTime, std::string label);
