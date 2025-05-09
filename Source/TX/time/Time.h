#pragma once
#include "TX/Platform.h"
#include "TX/time/Clock.h"
#include "TX/time/Duration.h"

namespace TX { 
constexpr uint64_t WALL_NANO_MASK = (1ULL << 30) - 1;
constexpr uint64_t MONO_NANO_MASK = (1ULL << 63) - 1;

class Time final {
 public:
  explicit Time() {
    auto wall = Clock::Real();
    auto mono = Clock::Monotonic();
    wall_ = (wall.sec << 30) | (wall.nsec);
    mono_ = (1ULL << 63) | (mono.sec * 1'000'000'000 + mono.nsec);
  }
  explicit Time(uint64_t nsec)
      : wall_(((nsec % 1'000'000'000) << 30) | (nsec % 1'000'000'000)),
        mono_(0) {}

  TX_NODISCARD uint64_t Unix() const { return sec(); }
  TX_NODISCARD uint64_t UnixMilli() const {
    return sec() * 1'000 + nsec() / 1'000'000;
  }
  TX_NODISCARD uint64_t UnixMicro() const {
    return sec() / 1'000'000 + nsec() / 1'000;
  }
  TX_NODISCARD uint64_t UnixNano() const {
    return sec() * 1'000'000'000 + nsec();
  }

  Time operator+(Duration d) {
    int64_t d_sec = d.inner / 1'000'000'000;
    int64_t d_nsec = d.inner % 1'000'000'000;
    uint64_t t_sec = sec();
    uint64_t t_nsec = nsec();
    wall_ = ((t_sec + d_sec) << 30) | (t_nsec + d_nsec);
    mono_ += d.inner;
    return *this;
  }
  Time operator-(Duration d) {
    int64_t d_sec = d.inner / 1'000'000'000;
    int64_t d_nsec = d.inner % 1'000'000'000;
    int64_t t_sec = sec();
    int64_t t_nsec = nsec();
    wall_ = ((t_sec - d_sec) << 30) | (t_nsec - d_nsec);
    mono_ -= d.inner;
    return *this;
  }
  Duration operator-(Time other) {
    uint64_t d;
    if (mono_ & other.mono_ & (MONO_NANO_MASK + 1))
      d = mono_ - other.mono_;
    else
      d = (sec() - other.sec()) * 1'000'000'000 + (nsec() + other.nsec());
    return Duration(d);
  }

 private:
  TX_NODISCARD int64_t sec() const { return static_cast<int64_t>(wall_ >> 30); }
  TX_NODISCARD int32_t nsec() const {
    return static_cast<int32_t>(wall_ & WALL_NANO_MASK);
  }

 private:
  uint64_t wall_;
  uint64_t mono_;
};

inline Time Now() { return Time(); }
inline Duration Since(Time t) { return Time() - t; }
inline Duration Until(Time t) { return t - Time(); }
}  // namespace TX
