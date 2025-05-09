#pragma once
#include <cstdint>

namespace TP {
class Task {
 public:
  explicit Task(int32_t id) : id_(id) {}
  [[nodiscard]] int32_t id() const { return id_; }

  enum class Kind : int {
    Auto,
  };

 private:
  int32_t id_;
};
}  // namespace TP
