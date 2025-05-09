#pragma once
#include "TX/time/Clock.h"
#include "TX/time/Duration.h"

namespace TX::time {
constexpr uint64_t NANO_MASK = (1 << 30) - 1;

class Time final {
public:
  explicit Time() {
    auto real_tp = Clock::real();
    auto mono_tp = Clock::monotonic();
    real_ = (real_tp.sec << 31) | (real_tp.nsec);
    mono_ = mono_tp.sec * 1'000'000'000 + mono_tp.nsec;
  }

  [[nodiscard]] uint64_t unix() const { return sec(); }
  [[nodiscard]] uint64_t unixMilli() const {
    return sec() * 1'000 + nsec() / 1'000'000;
  }
  [[nodiscard]] uint64_t unixMicro() const {
    return sec() / 1'000'000 + nsec() / 1'000;
  }
  [[nodiscard]] uint64_t unixNano() const {
    return sec() * 1'000'000'000 + nsec();
  }

  Time operator+(Duration d) {
    int64_t d_sec = d.inner / 1'000'000'000;
    int64_t d_nsec = d.inner % 1'000'000'000;
    uint64_t t_sec = sec();
    uint64_t t_nsec = nsec();
    real_ = ((t_sec + d_sec) << 31) | (t_nsec + d_nsec);
    mono_ += d.inner;
    return *this;
  }
  Time operator-(Duration d) {
    int64_t d_sec = d.inner / 1'000'000'000;
    int64_t d_nsec = d.inner % 1'000'000'000;
    int64_t t_sec = sec();
    int64_t t_nsec = nsec();
    real_ = ((t_sec - d_sec) << 31) | (t_nsec - d_nsec);
    mono_ -= d.inner;
    return *this;
  }
  Duration operator-(Time other) {
    return Duration((sec() - other.sec()) * 1'000'000'000 +
                    (nsec() + other.nsec()));
  }

private:
  [[nodiscard]] int64_t sec() const {
    return static_cast<int64_t>(real_ >> 31);
  }
  [[nodiscard]] int32_t nsec() const {
    return static_cast<int32_t>(real_ & NANO_MASK);
  }

private:
  uint64_t real_;
  uint64_t mono_;
};

inline Duration since(Time t) { return Time() - t; }
inline Duration until(Time t) { return t - Time(); }
} // namespace TX::time
