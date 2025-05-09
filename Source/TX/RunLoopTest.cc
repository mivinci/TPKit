#include <unordered_set>

#include "TX/RunLoop.h"
#include "TX/WaitGroup.h"
#include "gtest/gtest.h"

namespace TX {
class RunLoopTest : public testing::Test {};

TEST_F(RunLoopTest, Current) {
  constexpr int N = 3;
  std::unordered_set<RunLoop *> seen;
  std::vector<Own<Thread>> threads;
  WaitGroup wg;
  threads.reserve(N);
  wg.Add(N);
  for (int i = 0; i < N; ++i) {
    threads.push_back(Thread::Spawn([&] {
      seen.insert(RunLoop::Current());
      seen.insert(RunLoop::Current());
      wg.Done();
    }));
  }
  wg.Wait();
  EXPECT_EQ(N, seen.size());
}

TEST_F(RunLoopTest, Run) {
  RunLoop *loop = RunLoop::Current();
  EXPECT_EQ(loop, RunLoop::Main());
  EXPECT_EQ(loop->Run(), RunLoop::Status::Finished);
}
}  // namespace TX
