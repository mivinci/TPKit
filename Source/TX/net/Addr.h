#pragma once
#include "TX/Assert.h"
#include "TX/Bits.h"

namespace TX::net {
class IPv6Addr {
public:
  explicit IPv6Addr() = default;
  explicit IPv6Addr(const uint16 a, const uint16 b, const uint16 c,
                    const uint16 d, const uint16 e, const uint16 f,
                    const uint16 g, const uint16 h) {
    // ReSharper disable once CppDFAUnreachableCode
    if constexpr (TX_BIG_ENDIAN) {
      const uint16 octets[8] = {a, b, c, d, e, f, g, h};
      std::memcpy(octets_, octets, sizeof(octets));
    } else {
      const uint16 octets[8] = {swap(a), swap(b), swap(c), swap(d),
                                swap(e), swap(f), swap(g), swap(h)};
      std::memcpy(octets_, octets, sizeof(octets));
    }
  }

  [[nodiscard]] uint128 toBits() const {
    return uint128::fromBigEndianBytes(octets_);
  }
  static IPv6Addr fromBits(const uint128 bits) {
    IPv6Addr ip;
    bits.toBigEndianBytes(ip.octets_);
    return ip;
  }

private:
  uint8 octets_[16]{};
};

class IPv4Addr {
public:
  explicit IPv4Addr() = default;
  explicit IPv4Addr(const uint8 a, const uint8 b, const uint8 c,
                    const uint8 d) {
    octets_[0] = a;
    octets_[1] = b;
    octets_[2] = c;
    octets_[3] = d;
  }

  [[nodiscard]] bool isLoopBack() const { return octets_[0] == 127; }
  [[nodiscard]] bool isPrivate() const {
    return (octets_[0] == 10) ||
           (octets_[0] == 172 && octets_[1] >= 16 && octets_[1] <= 31) ||
           (octets_[0] == 192 && octets_[1] == 168);
  }
  [[nodiscard]] bool isLinkLocal() const {
    return (octets_[0] == 169 && octets_[1] == 254);
  }
  [[nodiscard]] bool isMulticast() const { return octets_[0] >= 224; }
  [[nodiscard]] bool isBroadcast() const { return octets_[0] == 255; }
  [[nodiscard]] bool isUnspecified() const { return octets_[0] == 0; }
  [[nodiscard]] bool isReserved() const {
    return (octets_[0] & 240) == 240 && !isBroadcast();
  }

  [[nodiscard]] uint8 operator[](const size_t index) const {
    return octets_[index];
  }

  [[nodiscard]] IPv6Addr toIPv6Compatible() const {
    const uint16 g = (octets_[0] << 8) | octets_[1];
    const uint16 h = (octets_[2] << 8) | octets_[3];
    return IPv6Addr(0, 0, 0, 0, 0, 0, g, h);
  }

  [[nodiscard]] IPv6Addr toIPv6Mapped() const {
    const uint16 g = (octets_[0] << 8) | octets_[1];
    const uint16 h = (octets_[2] << 8) | octets_[3];
    return IPv6Addr(0, 0, 0, 0, 0xFF, 0xFF, g, h);
  }

  [[nodiscard]] uint32 toBits() const {
    return (octets_[0] << 24) | (octets_[1] << 16) | (octets_[2] << 8) |
           octets_[3];
  }

  static IPv4Addr fromBits(const uint32 bits) {
    return IPv4Addr((bits >> 24) & 0xFF, (bits >> 16) & 0xFF,
                    (bits >> 8) & 0xFF, bits & 0xFF);
  }

private:
  uint8 octets_[4]{};
};

struct SocketAddrV4 {
  IPv4Addr ip;
  uint16 port;
  SocketAddrV4() : port(0) {}
};

struct SocketAddrV6 {
  IPv6Addr ip;
  uint16 port;
  SocketAddrV6() : port(0) {}
};

class SocketAddr {
public:
  explicit SocketAddr(const IPv4Addr &ip, const uint16 port) : is_v4_(true) {
    v4_.ip = ip;
    v4_.port = port;
  }
  explicit SocketAddr(const IPv6Addr &ip, const uint16 port) : is_v4_(false) {
    v6_.ip = ip;
    v6_.port = port;
  }

  static SocketAddr parse(const std::string &addr) { TX_TODO(); }

private:
  bool is_v4_;
  union {
    SocketAddrV4 v4_;
    SocketAddrV6 v6_;
  };
};
} // namespace TX::net
