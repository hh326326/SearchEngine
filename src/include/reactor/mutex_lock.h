/**
 * @file    :mutex_lock.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include <pthread.h>

namespace hh {
class MutexLock {
public:
  MutexLock();
  ~MutexLock();
  MutexLock(const MutexLock &) = delete;
  MutexLock &operator=(const MutexLock &) = delete;
  MutexLock(MutexLock &&) = default;
  MutexLock &operator=(MutexLock &&) = default;

  int Lock();
  int Unlock();

  pthread_mutex_t *GetMuextLockPtr() { return &_mutex; }

private:
  pthread_mutex_t _mutex;
};

class MutexLockGuard {
public:
  explicit MutexLockGuard(MutexLock &mutex) : _mutex(mutex) { _mutex.Lock(); }

  ~MutexLockGuard() { _mutex.Unlock(); }

private:
  MutexLock &_mutex;
};
}; // namespace hh

#endif
