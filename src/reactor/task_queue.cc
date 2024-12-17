/**
 * @file    :task_queue.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#include "reactor/task_queue.h"

namespace hh {
TaskQueue::TaskQueue(size_t queue_size)
    : _not_empty(_mutex), _not_full(_mutex), _queue_size(queue_size),
      _flag(true) {}

// TaskQueue::~TaskQueue() {}

bool TaskQueue::Empty() const {
  // return _que.size() == 0; // potential efficiency improvement
  return _que.empty();
}

bool TaskQueue::Full() const { return _que.size() == _queue_size; }

void TaskQueue::Push(Elem &&value) {
  MutexLockGuard auto_lock(_mutex); // autoLock栈对象

  while (Full()) //虚假唤醒
  {
    _not_full.Wait();
  }

  _que.push(std::move(value)); //队列中就有数据

  _not_empty.Notify();
}

Elem TaskQueue::Pop() {
  MutexLockGuard auto_lock(_mutex); // autoLock栈对象

  while (_flag && Empty()) {
    _not_empty.Wait(); //上半部与下半部
  }

  if (_flag) {
    Elem tmp = _que.front();
    _que.pop();

    _not_full.Notify();

    return tmp;
  }

  return nullptr;
}

void TaskQueue::Wakeup() {
  _flag = false;
  _not_empty.NotifyAll();
}

} // namespace hh