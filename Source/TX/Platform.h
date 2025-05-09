#pragma once

namespace TX {
#if defined(__GNUC__) || defined(__clang__)
#define TX_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#define TX_UNREACHABLE() __assume(0)
#else
#define TX_UNREACHABLE() ((void)0)
#endif

#if __cplusplus >= 201103L
#define TX_NORETURN [[noreturn]]
#elif defined(__GNUC__) || defined(__clang__)
#define TX_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define TX_NORETURN __declspec(noreturn)
#else
#define TX_NORETURN
#endif
} // namespace TX
