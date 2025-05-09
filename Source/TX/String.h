#pragma once
#include "TX/Platform.h"
#include "fmt/base.h"

namespace TX {
using String = std::string;
#if __cplusplus >= 201703L
using StringView = std::string_view;
#else
class StringView {
 public:
  StringView() : ptr_(nullptr), size_(0) {}
  StringView(const char *ptr) : ptr_(ptr), size_(strlen(ptr)) {}
  StringView(const char *ptr, const size_t size) : ptr_(ptr), size_(size) {}
  StringView(const char *beg, const char *end) : ptr_(beg), size_(end - beg) {}
  StringView(const StringView &other) = default;
  StringView(StringView &&other) noexcept
      : ptr_(other.ptr_), size_(other.size_) {
    other.ptr_ = nullptr;
    other.size_ = 0;
  }
  StringView &operator=(const char *ptr) {
    ptr_ = ptr;
    size_ = strlen(ptr);
    return *this;
  }
  StringView &operator=(const StringView &other) {
    if (this != &other) {
      ptr_ = other.ptr_;
      size_ = other.size_;
    }
    return *this;
  }
  StringView &operator=(StringView &&other) noexcept {
    if (this != &other) {
      ptr_ = other.ptr_;
      size_ = other.size_;
      other.ptr_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }
  ~StringView() = default;

  TX_NODISCARD size_t size() const { return size_; }
  TX_NODISCARD const char *data() const { return ptr_; }
  TX_NODISCARD bool empty() const { return size_ == 0; }
  TX_NODISCARD bool operator==(const StringView &other) const {
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
    char operator*() const { return *ptr_; }
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

template <>
struct fmt::formatter<TX::StringView> : formatter<string_view> {
  format_context::iterator format(const TX::StringView &str,
                                  format_context &ctx) const {
    return formatter<string_view>::format(string_view(str.data(), str.size()),
                                          ctx);
  }
};
#endif
}  // namespace TX
