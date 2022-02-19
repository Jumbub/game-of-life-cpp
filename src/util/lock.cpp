#pragma once

#include <iostream>
#include <thread>

// Simple fast mutex lock for pausing a worker thread from the main thread.
// Tangibly faster than the std::mutex.
struct Lock {
  // Request that the worker pause, spin lock until it has
  void waitForPause() {
    requestPause = true;
    while (running) {
      std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
  }

  // Resume worker
  void resume() {
    requestPause = false;
    while (!running) {
      std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
  }

  // Check for any pause requests, if there is, spin lock until un-paused
  void pauseIfRequested() {
    if (requestPause) {
      running = false;
      while (requestPause) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
      }
      running = true;
    }
  }

 private:
  bool requestPause = false;
  bool running = true;
};

// Helper class that maintains the lock for the duration of the scope.
// Usage: auto scope = LockForScope(board.lock);
struct LockForScope {
  LockForScope(Lock& lock) : lock(lock) { lock.waitForPause(); };
  ~LockForScope() { lock.resume(); };

 private:
  Lock& lock;
};
