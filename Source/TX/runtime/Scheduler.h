#pragma once
#include "TX/Assert.h"
#include "TX/runtime/Async.h"
#include "TX/runtime/Task.h"

namespace TX::runtime {
class Scheduler {
public:
  using Func = std::function<void()>;
  template <class T> void block(Async<T> &p) {}
};

} // namespace TX::runtime
