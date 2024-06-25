#ifndef SPINLOCK_
#define SPINLOCK_
#include <atomic>

class SpinLock {
 public:
  SpinLock() : ab_(false) {}

  void lock() {
    bool expected = false;
    while (!ab_.compare_exchange_weak(expected, true)) {
      expected = false;
    };
  }

  void unlock() { ab_.store(false); }

 private:
  std::atomic_bool ab_;
};

#endif