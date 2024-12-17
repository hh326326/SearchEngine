/**
 * @file    :thread.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#include "reactor/thread.h"
#include <cstdio>

namespace hh {
Thread::Thread(ThreadCallback &&cb)
    : _cb(std::move(cb)), _thid(0), _is_running(false) {}

Thread::~Thread() {
  if (_is_running) {
    int ret = pthread_detach(_thid);
    // TODO(seven): error handle
    if (ret) {
    }
  }
}

// unique_ptr<Thread> pthread(new MyThread());
int Thread::Start() // this
{
  // shift + k
  int ret = pthread_create(&_thid, nullptr, ThreadFunc, this);
  if (ret) {
    perror("pthread_create");
    return ret;
  }

  _is_running = true;
  return 0;
}

int Thread::Join() {
  int ret = 0;
  if (_is_running) {
    ret = pthread_join(_thid, nullptr);
    // TODO(seven): error handle
    _is_running = false;
  }
  return ret;
}

void *Thread::ThreadFunc(void *arg) {
  // Thread *pth = static_cast<Thread *>(arg);
  auto *pth = static_cast<Thread *>(arg);
  if (pth) {
    pth->_cb(); //回调函数
  }

  // return nullptr
  pthread_exit(nullptr);
}
} // namespace hh