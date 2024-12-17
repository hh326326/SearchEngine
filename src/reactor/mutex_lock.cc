/**
 * @file    :mutex_lock.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */
#include "reactor/mutex_lock.h"
#include <cstdio>

namespace hh {
MutexLock::MutexLock() {
  int ret = pthread_mutex_init(&_mutex, nullptr);
  if (ret) {
    perror("pthread_mutex_init");
    // TODO(seven): error handle
  }
}

MutexLock::~MutexLock() {
  int ret = pthread_mutex_destroy(&_mutex);
  if (ret) {
    perror("pthread_mutex_destroy");
    // TODO(seven): error handle
  }
}

int MutexLock::Lock() {
  int ret = pthread_mutex_lock(&_mutex);
  if (ret) {
    perror("pthread_mutex_lock");
  }
  return ret;
}

int MutexLock::Unlock() {
  int ret = pthread_mutex_unlock(&_mutex);
  if (ret) {
    perror("pthread_mutex_unlock");
  }
  return ret;
}
}; // namespace hh
