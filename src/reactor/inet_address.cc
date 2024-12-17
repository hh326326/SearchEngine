/**
 * @file    :inet_address.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#include "reactor/inet_address.h"
/* #include <sys/types.h> */
/* #include <sys/socket.h> */
#include <cstring>
/* #include <netinet/in.h> */

namespace hh {
InetAddress::InetAddress(const string &ip, unsigned short port) {
  // ::bzero(&_addr, sizeof(struct sockaddr_in));
  memset(&_addr, 0, sizeof(struct sockaddr_in));
  _addr.sin_family = AF_INET;
  _addr.sin_port = htons(port);
  _addr.sin_addr.s_addr = inet_addr(ip.c_str()); // host->network
}

InetAddress::InetAddress(const struct sockaddr_in &addr) : _addr(addr) {}

// InetAddress::~InetAddress() {} // rule of zero

string InetAddress::Ip() const { return string(inet_ntoa(_addr.sin_addr)); }

// unsigned short InetAddress::port() const
uint16_t InetAddress::Port() const { return ntohs(_addr.sin_port); }

const sockaddr_in *InetAddress::GetInetAddrPtr() const { return &_addr; }
} // namespace hh
