#pragma once
#include <functional>

namespace TX::runtime {
class Task {
public:
  using Func = std::function<void()>;
  explicit Task(Func f) : f_(std::move(f)), mandatory_(true) {}
  void run() { f_(); }
  void shutdown() {
    if (mandatory_)
      f_();
  }

private:
  Func f_;
  bool mandatory_;
};
} // namespace TX::thread
