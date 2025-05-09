#pragma once
#include "TX/net/Addr.h"
#include "TX/runtime/Async.h"

namespace TX::net {
using namespace runtime;
class TcpStream {
public:
  static Async<TcpStream> connect(const std::string &);
  [[nodiscard]] SocketAddr localAddr() const;
  [[nodiscard]] SocketAddr remoteAddr() const;
};

class TcpListener {
public:
  explicit TcpListener(const SocketAddr &);
  Async<TcpStream> accept();
  static Async<TcpListener> bind(const std::string &);
};

} // namespace TX::net
