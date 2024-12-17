/**
 * @file    :acceptor.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "reactor/inet_address.h"
#include "reactor/socket.h"
#include <string>

namespace hh {
using std::string;

class Acceptor {
public:
  Acceptor(const string &ip, unsigned short port);
  int Ready();
  int SetReuseAddr();
  int SetReusePort();
  int Bind();
  int Listen();
  int Accept();
  int Fd() const;

private:
  InetAddress _serv_addr;
  Socket _listen_sock;
};
}; // namespace hh

#endif
