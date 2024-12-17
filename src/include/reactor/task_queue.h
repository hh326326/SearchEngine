/**
 * @file    :task_queue.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __TASK_QUEUE_H__
#define __TASK_QUEUE_H__

#include "reactor/condition.h"
#include "reactor/mutex_lock.h"
#include "reactor/task.h"
#include <queue>

namespace hh {
using std::queue;

using Elem = function<void()>;

class TaskQueue {
public:
  explicit TaskQueue(size_t queue_size);
  // ~TaskQueue(); // rule of zero
  bool Empty() const;
  bool Full() const;
  void Push(Elem &&value);
  Elem Pop();

  //将所有的等在在_notEmpty上的线程唤醒
  void Wakeup();

private:
  queue<Elem> _que;     // sizeof(queue) = 80
  Condition _not_empty; // sizeof(Condition) = 56
  Condition _not_full;
  MutexLock _mutex;
  size_t _queue_size;
  bool _flag;
};
} // namespace hh
#endif
