/**
 * @file    :condition.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <pthread.h>

namespace hh {
class MutexLock; //前向声明

class Condition {
public:
  explicit Condition(MutexLock &mutex);
  ~Condition();
  Condition(const Condition &) = delete;
  Condition &operator=(const Condition &) = delete;
  Condition(Condition &&) = default;
  // Condition& operator=(Condition&&) = default;

  int Wait();
  int Notify();
  int NotifyAll();

private:
  pthread_cond_t _cond;
  MutexLock &_mutex;
};
}; // namespace hh

#endif
