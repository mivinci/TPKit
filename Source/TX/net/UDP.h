#pragma once
#include "TX/ByteArray.h"
#include "TX/Platform.h"
#include "TX/net/Addr.h"
#include "TX/net/Error.h"

namespace TX {
class UdpSocket {
 public:
  static Result<UdpSocket, NetError> Bind(const String &);
  Result<void, NetError> Connect(const String &);
  Result<int64, NetError> Read(ByteArray &buf);
  Result<int64, NetError> Write(const ByteArray &buf);
  TX_NODISCARD SocketAddr LocalAddr() const;
  TX_NODISCARD SocketAddr RemoteAddr() const;
};
}  // namespace TX
