/**
 * @file    :condition.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */
#include "reactor/condition.h"
#include "reactor/mutex_lock.h"

namespace hh {
Condition::Condition(MutexLock &mutex) : _mutex(mutex) {
  int ret = pthread_cond_init(&_cond, nullptr);
  // TODO(seven): error handle
  if (ret) {
  }
}

Condition::~Condition() {
  int ret = pthread_cond_destroy(&_cond);
  // TODO(seven): error handle
  if (ret) {
  }
}

int Condition::Wait() {
  int ret = pthread_cond_wait(&_cond, _mutex.GetMuextLockPtr()); //_mutex
  // TODO(seven): error handle
  return ret;
}

int Condition::Notify() {
  int ret = pthread_cond_signal(&_cond);
  // TODO(seven): error handle
  return ret;
}

int Condition::NotifyAll() {
  int ret = pthread_cond_broadcast(&_cond);
  // TODO(seven): error handle
  return ret;
}
}; // namespace hh