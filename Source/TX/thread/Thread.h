#pragma once
#include "TX/Assert.h"
#include "TX/Memory.h"
#include "TX/Own.h"
#include <functional>
#include <sys/errno.h>
#include <utility>
#ifdef _WIN32
#else
#include <pthread.h>
#endif

namespace TX::thread {
using Func = std::function<void()>;

class [[nodiscard]] Thread {
public:
  explicit Thread(Func f) : func_(std::move(f)), detached_(false) {
#ifdef _WIN32
#else
    TX_ASSERT_SYSCALL(pthread_create(&tid_, nullptr, &run, this));
#endif
  }
  ~Thread() {
    if (!detached_)
      join();
  }

  TX_DISALLOW_COPY(Thread)

  void join() {
#ifdef _WIN32
#else
    TX_ASSERT_SYSCALL(pthread_join(tid_, nullptr));
#endif
  }

  void detach() {
#ifdef _WIN32
#else
    TX_ASSERT_SYSCALL(pthread_detach(tid_));
    detached_ = true;
#endif
  }

  class Handle {
  public:
    explicit Handle(Thread *t) : t_(t) {}
    TX_DEFAULT_MOVE(Handle)
    TX_DISALLOW_COPY(Handle)
    void detach() { t_->detach(); }

  private:
    Own<Thread> t_;
  };

public:
  static Thread::Handle spawn(Func f) {
    return Thread::Handle(new Thread(std::move(f)));
  }

private:
  static void *run(void *arg) {
    auto *t = static_cast<Thread *>(arg);
    try {
      t->func_();
    } catch (...) {
      t->eptr_ = std::current_exception();
    }
    return nullptr;
  }

private:
  bool detached_;
  Func func_;
  std::exception_ptr eptr_;
#ifdef _WIN32
#else
  pthread_t tid_{};
#endif
};

} // namespace TX::thread
