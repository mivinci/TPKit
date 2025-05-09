#pragma once

#include <cstdint>

namespace TP {

struct TaskEvent {
  int32_t task_id;
  int32_t clip_no;
};

class TaskListener {
 public:
  virtual void onTaskEvent(TaskEvent event) = 0;
};
}  // namespace TP
