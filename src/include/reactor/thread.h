/**
 * @file    :thread.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __THREAD_H__
#define __THREAD_H__

#include <functional>
#include <pthread.h>
#include <string>

namespace hh {

extern __thread const char* name;
using std::function;

using ThreadCallback = function<void()>;

class Thread {
public:
  explicit Thread(ThreadCallback &&cb, const std::string& name);
  ~Thread();
  int Start();
  int Join();

private:
  //线程入口函数
  static void *ThreadFunc(void *arg);

private:
    //要去实现的任务
  ThreadCallback _cb; // sizeof(function<void()>) = 32
  pthread_t _thid;    // sizeof(pthread_t) = 8
  bool _is_running;
  std::string _name;
};
} // namespace hh

#endif
