#pragma once
#include "TX/Platform.h"
#include "TX/time/Clock.h"

namespace TX {
class Duration final {
 public:
  explicit Duration(int64_t n) : inner(n) {}
  static Duration NanoSecond(int n) { return Duration(n); }
  static Duration MicroSecond(int n) { return Duration(n * 1'000); }
  static Duration MilliSecond(int n) { return Duration(n * 1'000'000); }
  static Duration Second(int n) { return Duration(n * 1'000'000'000); }
  static Duration Minute(int n) { return Duration(n * 1'000'000'000 * 60); }
  static Duration Hour(int n) { return Duration(n * 1'000'000'000 * 60 * 60); }
  static Duration Forever() { return Duration(0x7FFFFFFFFFFFFFFF); }

  TX_NODISCARD int64_t NanoSeconds() const { return inner; }
  TX_NODISCARD int64_t MicroSeconds() const { return inner / 1'000; }
  TX_NODISCARD int64_t MilliSeconds() const { return inner / 1'000'000; }

  TX_NODISCARD double Seconds() const {
    int64_t sec = inner / 1'000'000'000;
    int64_t nsec = inner % 1'000'000'000;
    return static_cast<double>(sec) + static_cast<double>(nsec) / 1e9;
  }
  TX_NODISCARD double Minutes() const {
    int64_t min = inner / (60 * 1'000'000'000LL);
    int64_t nsec = inner % (60 * 1'000'000'000LL);
    return static_cast<double>(min) + static_cast<double>(nsec) / (60 * 1e9);
  }
  TX_NODISCARD double Hours() const {
    int64_t hour = inner / (60 * 60 * 1'000'000'000LL);
    int64_t nsec = inner % (60 * 60 * 1'000'000'000LL);
    return static_cast<double>(hour) +
           static_cast<double>(nsec) / (60 * 60 * 1e9);
  }

  TX_NODISCARD Duration Truncate(Duration m) const {
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

  TX_NODISCARD Clock::TimePoint ToTimePoint() const {
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

};  // namespace TX