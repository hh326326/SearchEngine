/**
  * @file    :socket_io.cc
  * @brief   :

  * @author  :hh
  * @version :V1.0.0
  * @date    :2024-12-17
  */
#include "reactor/socket_io.h"
namespace hh {
SocketIO::SocketIO(int fd) : _fd(fd) {}

ssize_t SocketIO::Readn(char *buf, size_t len) const {
  size_t left = len;
  char *pstr = buf;
  ssize_t ret = 0;

  while (left > 0) {
    ret = read(_fd, pstr, left);
    if (-1 == ret && errno == EINTR) {
      continue;
    }
    if (-1 == ret) {
      perror("read error -1");
      // BETTER: conversion check
      return static_cast<int64_t>(len) - ret;
    }
    if (0 == ret) {
      break;
    }

    pstr += ret;
    left -= static_cast<uint64_t>(ret);
  }

  return static_cast<int64_t>(len - left);
}

ssize_t SocketIO::ReadLine(char *buf, size_t len) const {
  size_t left = len - 1;
  char *pstr = buf;
  ssize_t ret = 0;
  ssize_t total = 0;

  while (left > 0) {
    //不会将缓冲区中的数据进行清空
    ret = recv(_fd, pstr, left, MSG_PEEK);
    if (-1 == ret && errno == EINTR) {
      continue;
    }
    if (-1 == ret) {
      perror("readLine error -1");
      return static_cast<int64_t>(len) - ret;
    }
    if (0 == ret) {
      break;
    }

    for (int idx = 0; idx < ret; ++idx) {
      if (pstr[idx] == '\n') {
        int sz = idx + 1;
        Readn(pstr, static_cast<uint64_t>(sz));
        pstr += sz;
        *pstr = '\0';

        return total + sz;
      }
    }

    Readn(pstr, static_cast<uint64_t>(ret)); //从内核态拷贝到用户态
    total += ret;
    pstr += ret;
    left -= static_cast<uint64_t>(ret);
  }
  *pstr = '\0';

  return total - static_cast<int64_t>(left);
}

// TODO(seven): handle conversion range check
ssize_t SocketIO::Writen(const char *buf, size_t len) const {
  size_t left = len;
  const char *pstr = buf;
  ssize_t ret = 0;

  while (left > 0) {
    ret = write(_fd, pstr, left);
    if (-1 == ret && errno == EINTR) {
      continue;
    }
    if (-1 == ret) {
      perror("writen error -1");
      return static_cast<int64_t>(len) - ret;
    }
    if (0 == ret) {
      break;
    }
    pstr += ret;
    left -= static_cast<uint64_t>(ret);
  }

  return static_cast<int64_t>(len - left);
}
} // namespace hh