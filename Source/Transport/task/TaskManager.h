#pragma once
#include <cstdint>

<<<<<<< HEAD
=======
#include "TX/Platform.h"
>>>>>>> f3c7944 (WIP)
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

  TX_NODISCARD int32_t CreateTask(Task::Kind kind = Task::Kind::Auto,
                                   TaskListener *listener = nullptr) const;
  TP_RESULT StartTask(int32_t task_id);
  TP_RESULT StopTask(int32_t task_id);
  TP_RESULT SuspendTask(int32_t task_id);
  TP_RESULT ResumeTask(int32_t task_id);
  TP_RESULT GetProxyURL(int32_t task_id) const;

 private:
  TX_NODISCARD int32_t GenTaskId(Task::Kind kind) const noexcept;

 private:
  int32_t factor_;
  int32_t span_;
};
}  // namespace TP
