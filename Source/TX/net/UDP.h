#pragma once
#include "TX/net/Addr.h"
#include "TX/runtime/Async.h"
#include <string>

namespace TX::net {
using namespace runtime;
class UdpSocket {
public:
  static Async<UdpSocket> bind(const std::string &);
  Async<void> connect(const std::string &);
  [[nodiscard]] SocketAddr localAddr() const;
  [[nodiscard]] SocketAddr remoteAddr() const;
};
} // namespace TX::net
