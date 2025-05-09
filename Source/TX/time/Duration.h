#pragma once
#include "TX/time/Clock.h"

namespace TX::time {
class Duration final {
public:
  explicit Duration(int64_t n) : inner(n) {}
  static Duration second(int n) { return Duration(n * 1'000'000'000); }
  static Duration minute(int n) { return Duration(n * 1'000'000'000 * 60); }
  static Duration hour(int n) { return Duration(n * 1'000'000'000 * 60 * 60); }

  [[nodiscard]] int64_t nanoSeconds() const { return inner; }
  [[nodiscard]] int64_t microSeconds() const { return inner / 1'000; }
  [[nodiscard]] int64_t milliSeconds() const { return inner / 1'000'000; }

  [[nodiscard]] double seconds() const {
    int64_t sec = inner / 1'000'000'000;
    int64_t nsec = inner % 1'000'000'000;
    return static_cast<double>(sec) + static_cast<double>(nsec) / 1e9;
  }
  [[nodiscard]] double minutes() const {
    int64_t min = inner / (60 * 1'000'000'000LL);
    int64_t nsec = inner % (60 * 1'000'000'000LL);
    return static_cast<double>(min) + static_cast<double>(nsec) / (60 * 1e9);
  }
  [[nodiscard]] double hours() const {
    int64_t hour = inner / (60 * 60 * 1'000'000'000LL);
    int64_t nsec = inner % (60 * 60 * 1'000'000'000LL);
    return static_cast<double>(hour) +
           static_cast<double>(nsec) / (60 * 60 * 1e9);
  }

  [[nodiscard]] Duration truncate(Duration m) const {
    return Duration(m.inner <= 0 ? inner : (inner - inner % m.inner));
  }

  Duration operator+(Duration &other) const {
    return Duration(inner + other.inner);
  }
  Duration operator-(Duration &other) const {
    return Duration(inner - other.inner);
  }
  Duration operator*(Duration &other) const {
    return Duration(inner * other.inner);
  }
  Duration operator/(Duration &other) const {
    return Duration(inner / other.inner);
  }
  Duration operator%(Duration &other) const {
    return Duration(inner % other.inner);
  }

  [[nodiscard]] Clock::TimePoint toTimePoint() const {
    Clock::TimePoint tp{};
    int64_t sec = inner / 1'000'000'000LL;
    int64_t nsec = inner % 1'000'000'000LL;
    tp.sec = sec;
    tp.nsec = nsec;
    return tp;
  }

private:
  friend class Time;
  int64_t inner;
};

}; // namespace TX::time