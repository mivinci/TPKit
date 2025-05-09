#pragma once
#include <cstddef>
#include <cstring>

#include "TX/Platform.h"
#include "fmt/base.h"

namespace TX {
class String {
 public:
  String() : ptr_(nullptr), size_(0) {}
  String(const char *ptr) : ptr_(ptr), size_(strlen(ptr)) {}
  String(const char *ptr, size_t size) : ptr_(ptr), size_(size) {}
  String(const char *beg, const char *end) : ptr_(beg), size_(end - beg) {}
  String(const String &other) = default;
  String(String &&other) noexcept : ptr_(other.ptr_), size_(other.size_) {
    other.ptr_ = nullptr;
    other.size_ = 0;
  }
  String &operator=(const char *ptr) {
    ptr_ = ptr;
    size_ = strlen(ptr);
    return *this;
  }
  String &operator=(const String &other) {
    if (this != &other) {
      ptr_ = other.ptr_;
      size_ = other.size_;
    }
    return *this;
  }
  String &operator=(String &&other) noexcept {
    if (this != &other) {
      ptr_ = other.ptr_;
      size_ = other.size_;
      other.ptr_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }
  ~String() = default;

  TX_NODISCARD size_t size() const { return size_; }
  TX_NODISCARD const char *data() const { return ptr_; }
  TX_NODISCARD bool empty() const { return size_ == 0; }
  TX_NODISCARD bool operator==(const String &other) const {
    return size_ == other.size_ && memcmp(ptr_, other.ptr_, size_) == 0;
  }
  TX_NODISCARD bool operator==(const char *other) const {
    return size_ == strlen(other) && memcmp(ptr_, other, size_) == 0;
  }

  class Iterator {
   public:
    explicit Iterator(const char *ptr) : ptr_(ptr) {}
    Iterator &operator++() {
      ++ptr_;
      return *this;
    }
    char operator*() { return *ptr_; }
    bool operator!=(const Iterator &other) const { return ptr_ != other.ptr_; }

   private:
    const char *ptr_;
  };

  TX_NODISCARD Iterator begin() const { return Iterator(ptr_); }
  TX_NODISCARD Iterator end() const { return Iterator(ptr_ + size_); }

 private:
  const char *ptr_;
  size_t size_;
};
}  // namespace TX

template <>
struct fmt::formatter<TX::String> : formatter<string_view> {
  format_context::iterator format(TX::String str, format_context &ctx) const {
    return formatter<string_view>::format(string_view(str.data(), str.size()),
                                          ctx);
  }
};
