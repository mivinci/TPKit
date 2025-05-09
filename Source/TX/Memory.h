#pragma once
namespace TX {
#define TX_DISALLOW_COPY(T)                                                    \
  T(const T &) = delete;                                                       \
  T &operator=(const T &) = delete;

#define TX_DEFAULT_MOVE(T)                                                     \
  T(T &&) = default;                                                           \
  T &operator=(T &&) = default;

template <typename T> inline void Drop(T &t) { t.~T(); }
} // namespace TX
