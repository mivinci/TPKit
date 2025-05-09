#include "TX/thread/Thread.h"
#include "TX/thread/Condvar.h"
#include <gtest/gtest.h>

namespace TX::thread {
TEST(ThreadTest, join) {
  int n = 114514;
  {
    Thread t([&]() { n = 42; });
  }
  EXPECT_EQ(n, 42);
}

TEST(ThreadTest, detach) {
  Condvar cv;
  Mutex<int> n(114514);
  Thread t([&]() {
    *(n.lock()) = 42;
    cv.notifyOne();
  });
  t.detach();
  auto n_guard = n.lock();
  cv.wait(n_guard);
  EXPECT_EQ(*n_guard, 42);
}

TEST(ThreadTest, spawn) {
  int n = 114514;
  {
    auto handle = Thread::spawn([&]() { n = 42; });
  }
  EXPECT_EQ(n, 42);
}

TEST(ThreadTest, Adder) {
  int n = 0, m = 3, N = 100;
  {
    std::vector<Thread::Handle> hs;
    hs.reserve(m);
    for (int i = 0; i < m; i++) {
      hs.push_back(Thread::spawn([&]() {
        for (int i = 0; i < N; i++) {
          n++;
        }
      }));
    }
  }
  EXPECT_EQ(n, N * m);
}
} // namespace TX::thread
