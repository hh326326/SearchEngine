/**
 * @file    :inet_address.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __INET_ADDRESS_H__
#define __INET_ADDRESS_H__

#include <arpa/inet.h>
#include <string>

namespace hh {
using std::string;

class InetAddress {
public:
  InetAddress(const string &ip, unsigned short port);
  explicit InetAddress(const struct sockaddr_in &addr);
  // ~InetAddress(); // rule of zero
  string Ip() const;
  uint16_t Port() const;
  const sockaddr_in *GetInetAddrPtr() const;

private:
  struct sockaddr_in _addr;
};
} // namespace hh

#endif
