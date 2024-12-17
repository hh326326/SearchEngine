/**
 * @file    :tcp_connection.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include "reactor/inet_address.h"
#include "reactor/socket.h"
#include "reactor/socket_io.h"
#include <functional>
#include <memory>

namespace hh {
using std::function;
using std::shared_ptr;

class EventLoop;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
  using TcpConnectionPtr = shared_ptr<TcpConnection>;
  using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
  TcpConnection(int fd, EventLoop *loop);
  // ~TcpConnection(); // rule of zero
  void Send(const string &msg);
  void SendInLoop(const string &msg);
  string Receive();
  string ToString();
  string GetPeerAddrString();
  bool IsClosed() const;

  //注册三个事件的回调函数
  void SetConnectionCallback(const TcpConnectionCallback &cb);
  void SetMessageCallback(const TcpConnectionCallback &cb);
  void SetCloseCallback(const TcpConnectionCallback &cb);

  //三个事件回调函数的执行
  void HandleConnectionCallback();
  void HandleMessageCallback();
  void HandleCloseCallback();

private:
  InetAddress GetLocalAddr();
  InetAddress GetPeerAddr();

  // sizeof(TcpConnectionCallback) = 32
  TcpConnectionCallback _on_connection_callback;
  TcpConnectionCallback _on_message_callback;
  TcpConnectionCallback _on_close_callback;
  EventLoop *_loop;
  Socket _sock;      // sizeof(Socket) = 4
  SocketIO _sock_io; // sizeof(SocketIO) = 4
  // order matters: InetAddress must be the last member of TcpConnection
  InetAddress _local_addr; // sizeof(InetAddress) = 16
  InetAddress _peer_addr;
};
} // namespace hh
#endif
