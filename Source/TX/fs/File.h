#pragma once
#include "TX/Bits.h"
#include "TX/Result.h"
#include "TX/String.h"
#include "TX/fs/Error.h"
#include "TX/runtime/Runtime.h"

namespace TX {
class File {
 public:
  explicit File() : fd_(-1) {};
  explicit File(int fd) : fd_(fd) {}

  static Async<File> Open(const String &path) {
    return Asyncify([&]() { return OpenSync(path); });
  }

  static File OpenSync(const String &path) {
    return File(0);
  }

 private:
  int fd_;
};
}  // namespace TX
