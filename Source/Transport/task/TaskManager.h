#pragma once
#include <cstdint>

#include "Transport/Result.h"
#include "Transport/task/Task.h"
#include "Transport/task/TaskListener.h"
#include "TX/Assert.h"

namespace TP {
#define TASK_ID_FACTOR 100000
#define TASK_ID_SPAN 1

class TaskManager {
 public:
  explicit TaskManager(int32_t factor = TASK_ID_FACTOR,
                       int8_t span = TASK_ID_SPAN)
      : factor_(factor), span_(span) {
    TX_ASSERT(TX::IsPowerOfTen(factor));
  }

  [[nodiscard]] int32_t createTask(Task::Kind kind = Task::Kind::Auto,
                                   TaskListener *listener = nullptr) const;
  TP_RESULT startTask(int32_t task_id);
  TP_RESULT stopTask(int32_t task_id);
  TP_RESULT suspendTask(int32_t task_id);
  TP_RESULT resumeTask(int32_t task_id);
  TP_RESULT getProxyURL(int32_t task_id, int32_t clip_no) const;

 private:
  [[nodiscard]] int32_t genTaskId(Task::Kind kind) const noexcept;

 private:
  int32_t factor_;
  int32_t span_;
};
}  // namespace TP
