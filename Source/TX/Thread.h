#pragma once
#include <cstddef>
#include <functional>
#include <utility>

#include "TX/Assert.h"
#include "TX/Memory.h"
#include "TX/Option.h"
#include "TX/Own.h"
#include "TX/Platform.h"
#include "TX/String.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace TX {
class TX_NODISCARD Thread {
 public:
  using Func = std::function<void()>;
  explicit Thread(Func f) : func_(std::move(f)), detached_(false) {
#ifdef _WIN32
    handle_ = CreateThread(NULL, 0, ThreadProc, this, CREATE_SUSPENDED, NULL);
    if (!handle_) TX_FATAL("CreateThread, error code {:d}", GetLastError());
    ResumeThread(handle_);
#else
    TX_ASSERT_SYSCALL(pthread_create(&tid_, nullptr, &Run, this));
#endif
  }
  
  ~Thread() {
    if (!detached_) Join();
  }

  TX_DISALLOW_COPY(Thread)

  void Join() {
#ifdef _WIN32
    if (WaitForSingleObject(handle_, INFINITE))
      TX_FATAL("WaitForSingleObject, error code {:d}", GetLastError());
    CloseHandle(handle_);
    handle_ = NULL;
#else
    TX_ASSERT_SYSCALL(pthread_join(tid_, nullptr));
#endif
  }

  void Detach() {
#ifdef _WIN32
#else
    TX_ASSERT_SYSCALL(pthread_detach(tid_));
#endif
    detached_ = true;
  }

  static Own<Thread> Spawn(Func f) {
    return Own<Thread>(new Thread(std::move(f)));
  }

 private:
  static void *Run(void *arg) {
    auto *t = static_cast<Thread *>(arg);
#ifdef _WIN32
#elif defined(__APPLE__)
    TX_ASSERT_SYSCALL(pthread_setname_np(t->name_.c_str()));
#elif defined(__linux__)
    TX_ASSERT_SYSCALL(pthread_setname_np(pthread_self(), t->name_.c_str()));
#else
#endif

    try {
      t->func_();
    } catch (...) {
      t->eptr_ = std::current_exception();
    }
    return nullptr;
  }

#ifdef _WIN32
  static DWORD WINAPI ThreadProc(LPVOID arg) {
    Run(static_cast<Thread *>(arg));
    return 0;
  }
#endif

 private:
  bool detached_;
  Func func_;
  std::string name_;
  std::exception_ptr eptr_;
#ifdef _WIN32
  HANDLE handle_;
#else
  pthread_t tid_;
#endif
};

}  // namespace TX
