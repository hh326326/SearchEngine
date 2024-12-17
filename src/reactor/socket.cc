/**
 * @file    :socket.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#include "reactor/socket.h"
#include <cstdio>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Socket::Socket() {
  _fd = ::socket(AF_INET, SOCK_STREAM, 0);
  // std::cout << "fd = " << _fd << '\n';
  if (_fd < 0) {
    perror("socket");
    return;
  }
}

Socket::Socket(int fd) : _fd(fd) {}

Socket::~Socket() { close(_fd); }

int Socket::Fd() const { return _fd; }

int Socket::ShutDownWrite() const {
  //关闭写端
  int ret = shutdown(_fd, SHUT_WR);
  if (ret) {
    perror("shutdown");
  }
  return ret;
}
