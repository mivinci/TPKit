#pragma once
#include <vector>

#include "TX/Bits.h"
#include "TX/Platform.h"
#include "TX/String.h"

namespace TX {
class ByteArray {
 public:
  ByteArray() = default;
  explicit ByteArray(size_t size) : buf_(size) {}
  explicit ByteArray(const String &str, bool zero_terminated = false)
      : buf_(str.size()) {
    ::memcpy(buf_.data(), str.data(), str.size());
    if (zero_terminated) {
      buf_.push_back(0);
    }
  }

  uint8_t &operator[](size_t index) { return buf_[index]; }
  const uint8_t &operator[](size_t index) const { return buf_[index]; }

  TX_NODISCARD size_t Size() const { return buf_.size(); }
  void Resize(size_t size) { buf_.resize(size); }
  uint8_t *Data() { return buf_.data(); }
  TX_NODISCARD const uint8_t *Data() const { return buf_.data(); }

 private:
  std::vector<uint8_t> buf_;
};
}  // namespace TX
