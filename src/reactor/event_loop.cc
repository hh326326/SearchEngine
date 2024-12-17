/**
 * @file    :event_loop.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */
#include "reactor/event_loop.h"
#include <sys/eventfd.h>
#include <unistd.h>

namespace hh {
EventLoop::EventLoop(Acceptor &acceptor)
    : _evtList(1024), _acceptor(acceptor), _epfd(CreateEpollFd()),
      _evtfd(CreateEventFd()), _is_looping(false) {
  AddEpollReadFd(acceptor.Fd()); //把listenfd放在红黑树上进行监听
  AddEpollReadFd(_evtfd);        //放在红黑树上进行监听
}

EventLoop::~EventLoop() {
  if (_epfd) {
    close(_epfd);
  }

  if (_evtfd) {
    close(_evtfd);
  }
}

void EventLoop::Loop() {
  _is_looping = true;
  while (_is_looping) {
    WaitEpollFd();
  }
}

void EventLoop::Unloop() { _is_looping = false; }

void EventLoop::RunInLoop(Functor &&cb) {
  //可以使用大括号将某些栈变量/栈对象提前结束
  {
    MutexLockGuard auto_lock(_mutex);
    _pengings_callback.push_back(std::move(cb));
  }

  //....
  //...
  Wakeup();
}

void EventLoop::DoPengingFunctors() {
  vector<Functor> tmp;
  {
    //粒度
    MutexLockGuard auto_lock(_mutex);
    tmp.swap(_pengings_callback);
  }

  // vector<Functor> _pengings_callback;
  for (auto &cb : tmp) {
    cb();
  }
}

ssize_t EventLoop::Wakeup() const {
  uint64_t one = 1;
  ssize_t ret = ::write(_evtfd, &one, sizeof(one));
  if (ret != sizeof(one)) {
    perror("write");
  }
  return ret;
}

ssize_t EventLoop::HandleRead() const {
  uint64_t one = 1;
  ssize_t ret = ::read(_evtfd, &one, sizeof(one));
  if (ret != sizeof(one)) {
    perror("read");
  }
  return ret;
}

void EventLoop::WaitEpollFd() {
  int ready_num = -1;
  do {
    ready_num = ::epoll_wait(_epfd, &*_evtList.begin(),
                             static_cast<int>(_evtList.size()), 5000);
  } while (-1 == ready_num && errno == EINTR);

  if (-1 == ready_num) {
    perror("ready_num == -1");
    return;
  }
  if (0 == ready_num) {
    printf(">>epoll_wait timeout\n");
  } else {
    if (ready_num == static_cast<int>(_evtList.size())) {
      _evtList.resize(
          2 * static_cast<uint64_t>(ready_num)); //考虑到了监听超过上限的问题
    }

    for (size_t idx = 0; static_cast<int>(idx) < ready_num; ++idx) {
      int fd = _evtList[idx].data.fd;
      if (fd == _acceptor.Fd()) //有新的连接请求进来
      {
        if (_evtList[idx].events & EPOLLIN) {
          HandleNewConnection();
        }
      } else if (fd == _evtfd) {
        HandleRead();
        DoPengingFunctors();
      } else {
        if (_evtList[idx].events & EPOLLIN) {
          HandleMessage(fd); //有数据的传输
        }
      }
    }
  }
}

void EventLoop::HandleNewConnection() {
  //只要peerfd有正确的返回值，就证明三次握手已经建立完成
  int peerfd = _acceptor.Accept();
  AddEpollReadFd(peerfd); //把peerfd放到红黑树上进行监听

  TcpConnectionPtr con(new TcpConnection(peerfd, this));

  // Tcp连接创建之后就可以进行三个事件的注册
  con->SetConnectionCallback(_on_connection_callback); //连接建立
  con->SetMessageCallback(_on_message_callback);       //消息的到达
  con->SetCloseCallback(_on_close_callback);           //连接的断开

  _conns.insert(std::make_pair(peerfd, con));
  con->HandleConnectionCallback();
}

void EventLoop::HandleMessage(int fd) {
  auto iter = _conns.find(fd);
  if (iter != _conns.end()) {
    //用Tcp连接里面定义一个函数
    // isClose,里面执行recv，recv的返回值等于0
    //
    bool flag = iter->second->IsClosed();
    if (flag) {
      //连接是断开的
      iter->second->HandleCloseCallback();
      DelEpollReadFd(fd); //将文件描述符从红黑树上删除
      _conns.erase(iter); //将文件描述符从map中删除

    } else {
      //连接是正常
      iter->second->HandleMessageCallback();
    }

  } else {
    /* cout << "该连接不存在" << endl; */
    printf("该连接不存在\n");
  }
}

int EventLoop::CreateEpollFd() {
  int fd = epoll_create(100);
  if (-1 == fd) {
    perror("epoll_create");
  }
  return fd;
}

int EventLoop::AddEpollReadFd(int fd) const {
  struct epoll_event evt;
  evt.events = EPOLLIN;
  evt.data.fd = fd;

  int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
  if (ret == -1) {
    perror("epoll_ctl add");
  }
  return ret;
}

int EventLoop::DelEpollReadFd(int fd) const {
  struct epoll_event evt;
  evt.events = EPOLLIN;
  evt.data.fd = fd;

  int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
  if (ret == -1) {
    perror("epoll_ctl del");
  }
  return ret;
}

void EventLoop::SetConnectionCallback(TcpConnectionCallback &&cb) {
  _on_connection_callback = std::move(cb);
}

void EventLoop::SetMessageCallback(TcpConnectionCallback &&cb) {
  _on_message_callback = std::move(cb);
}

void EventLoop::SetCloseCallback(TcpConnectionCallback &&cb) {
  _on_close_callback = std::move(cb);
}
int EventLoop::CreateEventFd() {
  int fd = eventfd(10, 0);
  if (-1 == fd) {
    perror("eventfd");
  }
  return fd;
}
} // namespace hh