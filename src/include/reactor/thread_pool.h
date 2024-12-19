/**
 * @file    :thread_pool.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "reactor/task_queue.h"
#include "reactor/thread.h"
#include <memory>
#include <vector>

namespace hh {
using std::unique_ptr;
using std::vector;

class ThreadPool {
public:
  ThreadPool(size_t thread_num, size_t queue_size);
  ~ThreadPool();

  void Start();
  void Stop();

  void AddTask(Task &&task);
  Task GetTask();

private:
  void ThreadFunc();

  TaskQueue _task_queue; // sizeof(TaskQueue) = 248
  vector<unique_ptr<Thread>> _threads;
  size_t _thread_num;
  size_t _queue_size; // used
  bool _isExit;
};
} // namespace hh

#endif
