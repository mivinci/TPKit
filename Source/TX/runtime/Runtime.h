#pragma once
#include "TX/runtime/Async.h"
#include "TX/runtime/Pool.h"
#include "TX/runtime/Scheduler.h"

namespace TX::runtime {
class Runtime {
private:
  Own<Scheduler> scheduler_;
  BlockingPool blocking_pool_;
};
} // namespace TX::runtime
