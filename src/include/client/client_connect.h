/**
 * @file    :client_connect.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#ifndef __CLIENT_CONNECT_H__
#define __CLIENT_CONNECT_H__

#include "reactor/inet_address.h"
#include "reactor/socket.h"
#include "reactor/socket_io.h"

#include <string>
#include <unistd.h>

namespace hh {
using std::string;

class ClientConnect {
public:
  ClientConnect(const std::string &ip, unsigned short port)
      : _sock(), _sockio(_sock.Fd()), _serverAddr(ip, port) {
    connect();
  }
  int getSockfd() const { return _sock.Fd(); }

  string recvMessage() {
    char buff[65535] = {0};
    read(_sock.Fd(), buff, sizeof(buff));

    return string(buff);
  }

  void sendMessage(const string &msg) {
    _sockio.Writen(msg.c_str(), msg.size());
  }

private:
  void connect() const {
    int ret = ::connect(_sock.Fd(), (sockaddr *)_serverAddr.GetInetAddrPtr(),
                        sizeof(*_serverAddr.GetInetAddrPtr()));
  }

public:
  Socket _sock;
  SocketIO _sockio;
  InetAddress _serverAddr;
};
} // namespace hh

#endif