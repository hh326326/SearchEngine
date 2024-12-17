/**
 * @file    :socket_io.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace hh {
class SocketIO {
public:
  explicit SocketIO(int fd);
  ssize_t Readn(char *buf, size_t len) const;
  ssize_t ReadLine(char *buf, size_t len) const;
  ssize_t Writen(const char *buf, size_t len) const;

private:
  int _fd;
};
} // namespace hh

#endif
