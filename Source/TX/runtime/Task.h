#pragma once
#include <coroutine>

#include "TX/Own.h"

namespace TX {
class Task {
 public:
  virtual void Run() = 0;

  template <class R>
  class Handle {
   public:
    template <class PromiseType>
    auto await_suspend(std::coroutine_handle<PromiseType> handle) {}
    bool await_ready() { return false; }
    R await_resume() {}

   private:
    friend class BlockingPool;
    explicit Handle(Task *task) : task_(task){};
    TX_DISALLOW_COPY(Handle)
    UnOwn<Task> task_;
  };
};
}  // namespace TX
