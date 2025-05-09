#pragma once
#include "TX/Clock.h"
#include "TX/Platform.h"

namespace TX {
class Duration final {
 public:
  explicit Duration(int64_t n) : inner_(n) {}
  static Duration NanoSecond(int n) { return Duration(n); }
  static Duration MicroSecond(int n) { return Duration(n * 1000); }
  static Duration MilliSecond(int n) { return Duration(n * 1000000); }
  static Duration Second(int n) { return Duration(n * 1000000000); }
  static Duration Minute(int n) { return Duration(n * 1000000000 * 60); }
  static Duration Hour(int n) { return Duration(n * 1000000000 * 60 * 60); }
  static Duration Forever() { return Duration(0x7FFFFFFFFFFFFFFF); }

  TX_NODISCARD int64_t NanoSeconds() const { return inner_; }
  TX_NODISCARD int64_t MicroSeconds() const { return inner_ / 1000; }
  TX_NODISCARD int64_t MilliSeconds() const { return inner_ / 1000000; }

  TX_NODISCARD double Seconds() const {
    int64_t sec = inner_ / 1000000000;
    int64_t nsec = inner_ % 1000000000;
    return static_cast<double>(sec) + static_cast<double>(nsec) / 1e9;
  }
  TX_NODISCARD double Minutes() const {
    int64_t min = inner_ / (60 * 1000000000LL);
    int64_t nsec = inner_ % (60 * 1000000000LL);
    return static_cast<double>(min) + static_cast<double>(nsec) / (60 * 1e9);
  }
  TX_NODISCARD double Hours() const {
    int64_t hour = inner_ / (60 * 60 * 1000000000LL);
    int64_t nsec = inner_ % (60 * 60 * 1000000000LL);
    return static_cast<double>(hour) +
           static_cast<double>(nsec) / (60 * 60 * 1e9);
  }

  TX_NODISCARD Duration Truncate(Duration m) const {
    return Duration(m.inner_ <= 0 ? inner_ : (inner_ - inner_ % m.inner_));
  }

  Duration operator+(const Duration &other) const {
    return Duration(inner_ + other.inner_);
  }
  Duration operator-(const Duration &other) const {
    return Duration(inner_ - other.inner_);
  }
  Duration operator*(const Duration &other) const {
    return Duration(inner_ * other.inner_);
  }
  Duration operator/(const Duration &other) const {
    return Duration(inner_ / other.inner_);
  }
  Duration operator%(const Duration &other) const {
    return Duration(inner_ % other.inner_);
  }
  bool operator==(const Duration &other) const { return inner_ == other.inner_; }

  TX_NODISCARD Clock::TimePoint ToTimePoint() const {
    Clock::TimePoint tp{};
    const int64_t sec = inner_ / 1000000000LL;
    const int64_t nsec = inner_ % 1000000000LL;
    tp.sec = sec;
    tp.nsec = nsec;
    return tp;
  }

  static Duration FOREVER;

 private:
  friend class Time;
  int64_t inner_;
};

inline Duration Duration::FOREVER = Duration(INT64_MAX);

};  // namespace TX