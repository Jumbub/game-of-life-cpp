#include <iostream>
#include <thread>
#include "lock.h"

void Lock::waitForPause() {
  requestPause = true;
  while (running) {
    std::this_thread::sleep_for(std::chrono::microseconds(1));
  }
}

void Lock::resume() {
  requestPause = false;
  while (!running) {
    std::this_thread::sleep_for(std::chrono::microseconds(1));
  }
}

bool Lock::pauseIfRequested() {
  if (requestPause) {
    running = false;
    while (requestPause) {
      std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
    running = true;
    return true;
  }
  return false;
}

LockForScope::LockForScope(Lock& lock) : lock(lock) {
  lock.waitForPause();
}

LockForScope::~LockForScope() {
  lock.resume();
}
