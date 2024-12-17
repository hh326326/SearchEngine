/**
 * @file    :tcp_connection.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#include "reactor/tcp_connection.h"
#include "reactor/event_loop.h"
#include <iostream>
#include <sstream>

namespace hh {
// using std::cout;
// using std::endl;
using std::ostringstream;

TcpConnection::TcpConnection(int fd, EventLoop *loop)
    : _loop(loop), _sock(fd), _sock_io(fd), _local_addr(GetLocalAddr()),
      _peer_addr(GetPeerAddr()) {}

// TcpConnection::~TcpConnection() {}

void TcpConnection::Send(const string &msg) {
  _sock_io.Writen(msg.c_str(), msg.size());
}

void TcpConnection::SendInLoop(const string &msg) {
  if (_loop) {
    // _loop->runInLoop(std::bind(&TcpConnection::Send, this,
    // msg));//注册到EventLoop
    _loop->RunInLoop([this, msg] { Send(msg); }); //注册到EventLoop
    // void runInLoop(function<void()> &&cb);
  }
}

string TcpConnection::Receive() {
  char buff[65535] = {0};
  _sock_io.ReadLine(buff, sizeof(buff));

  return string(buff);
}

string TcpConnection::ToString() {
  ostringstream oss;
  oss << _local_addr.Ip() << ":" << _local_addr.Port() << "---->"
      << _peer_addr.Ip() << ":" << _peer_addr.Port();

  return oss.str();
}

// add by jifenghi
string TcpConnection::GetPeerAddrString() {
  ostringstream oss;
  oss << _peer_addr.Ip() << ":" << _peer_addr.Port();
  return oss.str();
}

bool TcpConnection::IsClosed() const {
  char buf[10] = {};
  ssize_t ret = ::recv(_sock.Fd(), buf, sizeof(buf), MSG_PEEK);

  return (ret == 0);
}

//获取本端的网络地址信息
InetAddress TcpConnection::GetLocalAddr() {
  struct sockaddr_in addr;
  socklen_t len = sizeof(struct sockaddr);
  int ret =
      getsockname(_sock.Fd(), reinterpret_cast<struct sockaddr *>(&addr), &len);
  if (-1 == ret) {
    perror("getsockname");
  }

  return InetAddress(addr);
}

//获取对端的网络地址信息
InetAddress TcpConnection::GetPeerAddr() {
  struct sockaddr_in addr;
  socklen_t len = sizeof(struct sockaddr);
  int ret =
      getpeername(_sock.Fd(), reinterpret_cast<struct sockaddr *>(&addr), &len);
  if (-1 == ret) {
    perror("getpeername");
  }

  return InetAddress(addr);
}

void TcpConnection::SetConnectionCallback(const TcpConnectionCallback &cb) {
  _on_connection_callback = std::move(cb);
}

void TcpConnection::SetMessageCallback(const TcpConnectionCallback &cb) {
  _on_message_callback = std::move(cb);
}

void TcpConnection::SetCloseCallback(const TcpConnectionCallback &cb) {
  _on_close_callback = std::move(cb);
}

//三个事件回调函数的执行
void TcpConnection::HandleConnectionCallback() {
  if (_on_connection_callback) {
    /* using TcpConnectionPtr = shared_ptr<TcpConnection>; */
    /* using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
     */
    /* TcpConnectionCallback _on_connection_callback; */
    // function<void(const shared_ptr<TcpConnection> &)>
    // _on_connection_callback;
    _on_connection_callback(shared_from_this());
  }
}
void TcpConnection::HandleMessageCallback() {
  if (_on_message_callback) {
    _on_message_callback(shared_from_this());
  }
}
void TcpConnection::HandleCloseCallback() {
  if (_on_close_callback) {
    _on_close_callback(shared_from_this());
  }
}
} // namespace hh