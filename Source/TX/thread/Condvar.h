#pragma once
#include "Mutex.h"
#include "TX/Option.h"
#include "TX/time/Time.h"

namespace TX::thread {
class Condvar {
public:
  explicit Condvar() {
#ifdef _WIN32
    InitializeConditionVariable(&cv);
#else
    TX_ASSERT_SYSCALL(pthread_cond_init(&cv_, nullptr));
#endif
  }
  void notifyOne() {
#ifdef _WIN32
    WakeConditionVariable(&cv);
#else
    TX_ASSERT_SYSCALL(pthread_cond_signal(&cv_));
#endif
  }
  void notifyAll() {
#ifdef _WIN32
    WakeAllConditionVariable(&cv);
#else
    TX_ASSERT_SYSCALL(pthread_cond_broadcast(&cv_));
#endif
  }
  template <typename T>
  void wait(MutexGuard<T> &guard, Option<time::Duration> timeout = None) {
    if (timeout.isSome()) {
#ifdef _WIN32
      DWORD ms = timeout.unwrap().milliSeconds();
      SleepConditionVariableCS(&cv, &guard.lock->cs, ms);
#else
      time::Clock::TimePoint tp = timeout.unwrap().toTimePoint();
      TX_ASSERT_SYSCALL(pthread_cond_timedwait(
          &cv_, &guard.lock_->inner_, reinterpret_cast<const timespec *>(&tp)));
#endif
    } else {
#ifdef _WIN32
      SleepConditionVariableCS(&cv, &guard.lock->cs, INFINITE);
#else
      TX_ASSERT_SYSCALL(pthread_cond_wait(&cv_, &guard.lock_->inner_));
#endif
    }
  }
#ifdef _WIN32
  CONDITION_VARIABLE cv;
#else
  pthread_cond_t cv_{};
#endif
};
} // namespace TX::thread
