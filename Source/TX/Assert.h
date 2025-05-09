#pragma once
#include "TX/Platform.h"
#include "TX/log/Log.h"

namespace TX {
#define TX_ASSERT(expr, ...)            \
  do {                             \
    if (expr) break;               \
    TX_FATAL("assertion: " #expr ", " ##__VA_ARGS__); \
  } while (0)

#define TX_ASSERT_SYSCALL(expr)                                      \
  do {                                                               \
    int status = expr;                                               \
    if (status == 0) break;                                          \
    TX_FATAL("assertion(syscall): " #expr " returned {:d}", status); \
  } while (0)

#define TX_TODO(...)                \
  do {                              \
    TX_FATAL("TODO: " __VA_ARGS__); \
    TX_UNREACHABLE();               \
  } while (0)

inline constexpr bool IsPowerOfTen(int32_t n) {
  if (n < 10) return false;
  while (n % 10 == 0) n /= 10;
  return n == 1;
}
}  // namespace TX
