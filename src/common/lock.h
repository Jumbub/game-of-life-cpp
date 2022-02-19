#pragma once

// Simple fast mutex lock for pausing a worker thread from the main thread.
// Tangibly faster than the std::mutex.
struct Lock {
  // Request that the worker pause, spin lock until it has
  void waitForPause();

  // Resume worker
  void resume();

  // Check for any pause requests, if there is, spin lock until un-paused
  void pauseIfRequested();

 private:
  bool requestPause = false;
  bool running = true;
};

// Helper class that maintains the lock for the duration of the scope.
// Usage: auto scope = LockForScope(board.lock);
struct LockForScope {
  LockForScope(Lock& lock);
  ~LockForScope();

 private:
  Lock& lock;
};
