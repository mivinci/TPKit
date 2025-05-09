#pragma once
#include "TX/Bits.h"
#include "TX/ByteArray.h"
#include "TX/Platform.h"
#include "TX/Result.h"
#include "TX/String.h"
#include "TX/net/Addr.h"
#include "TX/net/Error.h"

namespace TX {
class TcpStream {
 public:
  Result<int64, NetError> Read(ByteArray &buf);
  Result<int64, NetError> Write(const ByteArray &buf);
  TX_NODISCARD SocketAddr LocalAddr() const;
  TX_NODISCARD SocketAddr RemoteAddr() const;
  static Result<TcpStream, NetError> Connect(const String &addr);
};

class TcpListener {
 public:
  explicit TcpListener(const SocketAddr &addr);
  Result<TcpStream, NetError> Accept();
  TX_NODISCARD SocketAddr Addr() const;
  static Result<TcpListener, NetError> Bind(const String &addr);
};

}  // namespace TX
