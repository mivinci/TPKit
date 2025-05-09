#include "TX/runtime/Pool.h"

namespace TX::runtime {
void BlockingPool::spawn(Func f) {
  auto task = Task(std::move(f));
  auto shared = shared_.lock();
  shared->queue.push(std::move(task));
  if (shared->num_idle_threads > 0) {
    shared->num_idle_threads--;
    cv_.notifyOne();
    return;
  }
  if (shared->num_threads >= max_threads_)
    return;
  int id = shared->num_threads;
  shared->threads.emplace(id, new Thread([this, id]() { run(id); }));
  shared->num_threads++;
}

void BlockingPool::run(int) {
  auto shared = shared_.lock();
  while (true) {
    while (!shared->queue.empty()) {
      Task task = shared->queue.front();
      shared->queue.pop();
      drop(shared);
      task.run();
      shared = shared_.lock();
    }

    if (shared->shutdown) {
      while (!shared->queue.empty()) {
        Task task = shared->queue.front();
        shared->queue.pop();
        drop(shared);
        task.shutdown();
        shared = shared_.lock();
      }
      return;
    }

    shared->num_idle_threads++;
    cv_.wait(shared);
  }
}

void BlockingPool::shutdown() {
  std::vector<Thread::Handle> threads;
  {
    auto shared = shared_.lock();
    if (shared->shutdown)
      return;
    shared->shutdown = true;
    // We have to first release the lock and then notify and join all worker
    // threads before returning to the caller of shutdown.
    for (auto &t : shared->threads) {
      threads.push_back(std::move(t.second));
    }
  }
  cv_.notifyAll();
}
} // namespace TX::thread
