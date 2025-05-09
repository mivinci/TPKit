#pragma once
#include "TX/Bits.h"

namespace TP::task {
class Task {
public:
  explicit Task(int32_t id) : id_(id) {}
  [[nodiscard]] int32_t Id() const { return id_; }
  enum class Kind : int32_t {
    Auto = 1,
  };

private:
  int32_t id_;
};
} // namespace TP::task
