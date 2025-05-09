#pragma once

#include <queue>
#include <unordered_map>

#include "TX/Function.h"
#include "TX/Own.h"
#include "TX/runtime/Task.h"
#include "TX/sync/Condvar.h"
#include "TX/sync/Mutex.h"
#include "TX/thread/Thread.h"

namespace TX {

template <class Func>
class BlockingTask : public Task {
 public:
  using Output = ReturnType<Func>;
  explicit BlockingTask(Func f) : f_(std::move(f)) {}
  void Run() override { f_(); }

 private:
  Func f_;
};

class UnownedTask {
 public:
  explicit UnownedTask(Task *task, bool mandatory = true)
      : task_(task), mandatory_(mandatory) {}
  void Run() {}
  void Shutdown() {
    if (mandatory_) Run();
  }

 private:
  UnOwn<Task> task_;
  bool mandatory_;
};

class BlockingPool {
 public:
  explicit BlockingPool(int max_threads = 3) : max_threads_(max_threads) {}
  ~BlockingPool() { Shutdown(); }

  TX_DISALLOW_COPY(BlockingPool)
  void Shutdown();

  template <class F, class R>
  Task::Handle<R> Spawn(F f, bool mandatory = true) {
    auto blocking_task = new BlockingTask(std::move(f));
    SpawnTask(UnownedTask(blocking_task, mandatory));
    return Task::Handle<R>(blocking_task);
  }

  struct Shared {
    std::queue<UnownedTask> queue;
    std::unordered_map<int, Own<Thread>> threads;
    int num_threads;
    int num_idle_threads;
    bool shutdown;
    explicit Shared() : num_threads(0), num_idle_threads(0), shutdown(false) {}
  };

 private:
  void Run(int);
  void SpawnTask(UnownedTask &&task);

 private:
  int max_threads_;
  Condvar cond_;
  Mutex<Shared> shared_;
};
}  // namespace TX
