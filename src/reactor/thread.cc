/**
 * @file    :thread.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#include "reactor/thread.h"
#include <cstddef>
#include <cstdio>
#include <pthread.h>


namespace hh {

__thread const char* name = nullptr;

Thread::Thread(ThreadCallback &&cb, const std::string& name)
    : _cb(std::move(cb)), _thid(0), _is_running(false) , _name(name){}

Thread::~Thread() {
  if (_is_running) {
    int ret = pthread_detach(_thid);
    _is_running = false;
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
    // delete name;
  }
  return ret;
}

void *Thread::ThreadFunc(void *arg) {
  // Thread *pth = static_cast<Thread *>(arg);
  auto *pth = static_cast<Thread *>(arg);
  hh::name = pth->_name.c_str();
  if (pth) {
    pth->_cb(); //回调函数
  }

  // return nullptr
  pthread_exit(nullptr);
}
} // namespace hh