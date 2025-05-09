#pragma once
#include "TX/Assert.h"
#include "Transport/Error.h"
#include "Transport/task/Task.h"
#include <cstdint>

namespace TP::task {
#define TASK_ID_FACTOR 100000
#define TASK_ID_SPAN 1

class TaskManager {
public:
  explicit TaskManager(int32_t factor = TASK_ID_FACTOR,
                       int8_t span = TASK_ID_SPAN)
      : factor_(factor), span_(span) {
    TX_ASSERT(TX::isPowerOfTen(factor));
  }

  [[nodiscard]] int32_t createTask(Task::Kind kind = Task::Kind::Auto) const;
  Status startTask(int32_t task_id);
  Status stopTask(int32_t task_id);

private:
  [[nodiscard]] int32_t genTaskId(Task::Kind kind) const noexcept;

private:
  int32_t factor_;
  int32_t span_;
};
} // namespace TP::task
