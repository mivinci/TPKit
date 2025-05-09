#include "Transport/task/TaskManager.h"

namespace TP::task {

int32_t TaskManager::genTaskId(Task::Kind kind) const noexcept {
  static std::atomic<int32_t> genTaskId = 0;
  int32_t offset = factor_ * static_cast<int32_t>(kind);
  int32_t base = genTaskId.fetch_add(span_, std::memory_order_relaxed);
  return (base >= factor_) ? -1 : base + offset;
}

int32_t TaskManager::createTask(Task::Kind kind) const {
  int32_t task_id = genTaskId(kind);
  if (task_id < 0)
    return -1;

  return task_id;
}

Status TaskManager::startTask(const int32_t task_id) {
  return Status::Ok;
}

Status TaskManager::stopTask(int32_t task_id) { return Status::Ok; }
} // namespace TP::task
