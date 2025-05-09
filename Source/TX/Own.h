#pragma once
#include "TX/Memory.h"
#include "TX/Platform.h"

namespace TX {
template <typename T> class Own {
public:
  explicit Own() : t_(nullptr) {}
  explicit Own(T *t) : t_(t) {}
  TX_DISALLOW_COPY(Own)
  Own(Own &&other) noexcept {
    t_ = other.t_;
    other.t_ = nullptr;
  }
  Own &operator=(Own &&other) noexcept {
    t_ = other.t_;
    other.t_ = nullptr;
    return *this;
  }
  ~Own() {
    delete t_;
    t_ = nullptr;
  }

  T &operator*() { return *t_; }
  T *operator->() { return t_; }

private:
  T *t_;
};
} // namespace TX
