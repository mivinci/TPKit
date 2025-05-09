#pragma once
#include <vector>

#include "TX/Bits.h"
#include "TX/Platform.h"

namespace TX {
class ByteArray {
 public:
  ByteArray() = default;
  explicit ByteArray(size_t size) : data_(size) {}
  ByteArray(const ByteArray &) = default;
  ByteArray(ByteArray &&) = default;
  ByteArray &operator=(const ByteArray &) = default;
  ByteArray &operator=(ByteArray &&) = default;

  uint8_t &operator[](size_t index) { return data_[index]; }
  const uint8_t &operator[](size_t index) const { return data_[index]; }

  TX_NODISCARD size_t Size() const { return data_.size(); }
  void Resize(size_t size) { data_.resize(size); }
  uint8_t *Data() { return data_.data(); }
  TX_NODISCARD const uint8_t *Data() const { return data_.data(); }

 private:
  std::vector<uint8_t> data_;
};
}  // namespace TX
