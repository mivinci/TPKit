#include "Transport/task/TaskManager.h"
#include "gtest/gtest.h"

namespace TP {
class TaskManagerTest : public testing::Test {};

TEST_F(TaskManagerTest, genTaskId) {
  const int factor = 10, span = 1;
  TaskManager mgr(factor, span);
  for (int i = 0; i < factor; i++) {
    int32_t id = mgr.createTask(Task::Kind::Auto);
    EXPECT_EQ(id / factor, static_cast<int32_t>(Task::Kind::Auto));
    EXPECT_EQ(id % factor, i);
  }
  EXPECT_EQ(mgr.createTask(), -1);
}

}  // namespace TP
