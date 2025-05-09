#pragma once
#include "TX/ByteArray.h"
#include "TX/Platform.h"
#include "TX/String.h"

namespace TX {
class Path {
 public:
  explicit Path(const String &path) : buf_(path, true) {}
  TX_NODISCARD const char *ToCStr() const {
    return reinterpret_cast<const char *>(buf_.Data());
  }

 private:
  ByteArray buf_;
};
}  // namespace TX
