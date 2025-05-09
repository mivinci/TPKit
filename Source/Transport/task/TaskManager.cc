#include "Transport/task/TaskManager.h"

#include <atomic>

#include "Transport/task/TaskListener.h"

namespace TP {

int32_t TaskManager::genTaskId(Task::Kind kind) const noexcept {
  static std::atomic<int32_t> genTaskId = 0;
  int32_t offset = factor_ * static_cast<int32_t>(kind);
  int32_t base = genTaskId.fetch_add(span_, std::memory_order_relaxed);
  return (base >= factor_) ? -1 : base + offset;
}

int32_t TaskManager::createTask(Task::Kind kind, TaskListener *listener) const {
  int32_t task_id = genTaskId(kind);
  return task_id;
}

TP_RESULT TaskManager::startTask(const int32_t task_id) { return TP_OK; }

TP_RESULT TaskManager::stopTask(int32_t task_id) { return TP_OK; }

TP_RESULT TaskManager::suspendTask(int32_t task_id) { return TP_OK; }

TP_RESULT TaskManager::resumeTask(int32_t task_id) { return TP_OK; }

TP_RESULT TaskManager::getProxyURL(int32_t task_id, int32_t clip_no) const {
  return TP_OK;
}
}  // namespace TP
