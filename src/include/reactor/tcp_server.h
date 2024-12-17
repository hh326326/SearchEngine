/**
 * @file    :tcp_server.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */

#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "reactor/acceptor.h"
#include "reactor/event_loop.h"
#include "reactor/tcp_connection.h"
#include <iostream>
#include <unistd.h>

namespace hh {
class TcpServer {
public:
  TcpServer(const string &ip, unsigned short port)
      : _acceptor(ip, port), _loop(_acceptor) {}

  void Start() {
    _acceptor.Ready();
    _loop.Loop();
  }

  void Stop() { _loop.Unloop(); }

  void SetAllCallback(TcpConnectionCallback &&onConnection,
                      TcpConnectionCallback &&onMessage,
                      TcpConnectionCallback &&onClose) {
    _loop.SetConnectionCallback(std::move(onConnection));
    _loop.SetMessageCallback(std::move(onMessage));
    _loop.SetCloseCallback(std::move(onClose));
  }

private:
  // order matters: _acceptor has to be initialized
  // before _loop
  Acceptor _acceptor;
  EventLoop _loop;
};
} // namespace hh

#endif
