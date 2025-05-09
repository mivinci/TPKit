#pragma once

#include "TX/runtime/Task.h"
#include "TX/sync/Condvar.h"
#include "TX/sync/Mutex.h"
#include "TX/thread/Thread.h"
#include <functional>
#include <queue>
#include <unordered_map>

namespace TX {

class BlockingPool {
public:
  using Func = std::function<void()>;
  explicit BlockingPool(int max_threads = 2) : max_threads_(max_threads) {}
  ~BlockingPool() { shutdown(); }

  void spawn(Func f);
  void shutdown();

  struct Shared {
    std::queue<Task> queue;
    std::unordered_map<int, Thread::Handle> threads;
    int num_threads;
    int num_idle_threads;
    bool shutdown;
    explicit Shared() : num_threads(0), num_idle_threads(0), shutdown(false) {}
  };

private:
  void run(int);

private:
  int max_threads_;
  Mutex<Shared> shared_;
  Condvar cv_;
};
} // namespace TX
