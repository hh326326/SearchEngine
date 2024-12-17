/**
 * @file    :event_loop.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "reactor/acceptor.h"
#include "reactor/mutex_lock.h"
#include "reactor/tcp_connection.h"
#include <functional>
#include <map>
#include <memory>
#include <sys/epoll.h>
#include <vector>

namespace hh {
using std::function;
using std::map;
using std::shared_ptr;
using std::vector;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
using Functor = function<void()>;

class EventLoop {
public:
  explicit EventLoop(Acceptor &acceptor);
  ~EventLoop();
  void Loop();
  void Unloop();

  void RunInLoop(Functor &&cb);

  ssize_t Wakeup() const;
  ssize_t HandleRead() const;

  void DoPengingFunctors();

  void SetConnectionCallback(TcpConnectionCallback &&cb);
  void SetMessageCallback(TcpConnectionCallback &&cb);
  void SetCloseCallback(TcpConnectionCallback &&cb);

private:
  void WaitEpollFd();
  void HandleNewConnection();
  void HandleMessage(int fd);
  static int CreateEpollFd();
  int AddEpollReadFd(int fd) const;
  int DelEpollReadFd(int fd) const;
  static int CreateEventFd();

  /* map<int, shared_ptr<TcpConnection>> _conns; */
  map<int, TcpConnectionPtr> _conns;
  MutexLock _mutex;
  TcpConnectionCallback _on_connection_callback;
  TcpConnectionCallback _on_message_callback;
  TcpConnectionCallback _on_close_callback;
  vector<struct epoll_event> _evtList;
  vector<Functor> _pengings_callback;
  Acceptor &_acceptor;
  int _epfd; //红黑树的根节点
  int _evtfd;
  bool _is_looping;
};
} // namespace hh
#endif
