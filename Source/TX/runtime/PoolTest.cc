#include "TX/runtime/Pool.h"
#include "gtest/gtest.h"

namespace TX::runtime {
TEST(BlockingPoolTest, Simple1) {
  BlockingPool pool(1);
  pool.spawn([]() {});
  pool.spawn([]() {});
}

TEST(BlockingPoolTest, Simple2) {
  BlockingPool pool(2);
  pool.spawn([]() {});
  pool.spawn([]() {});
}

TEST(BlockingPoolTest, Adder1) {
  int N = 42;
  std::atomic<int> n = 0;
  {
    BlockingPool pool(1);
    for (int i = 0; i < 42; i++) {
      pool.spawn([&]() { n.fetch_add(1); });
    }
  }
  EXPECT_EQ(n.load(), N);
}

TEST(BlockingPoolTest, Adder2) {
  int N = 42;
  std::atomic<int> n = 0;
  {
    BlockingPool pool(2);
    for (int i = 0; i < 42; i++) {
      pool.spawn([&]() { n.fetch_add(1); });
    }
  }
  EXPECT_EQ(n.load(), N);
}

TEST(BlockingPoolTest, Adder3) {
  int N = 42;
  std::atomic<int> n = 0;
  {
    BlockingPool pool(3);
    for (int i = 0; i < 42; i++) {
      pool.spawn([&]() { n.fetch_add(1); });
    }
  }
  EXPECT_EQ(n.load(), N);
}
} // namespace TX::thread
