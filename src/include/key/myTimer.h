/**
 * @file    :myTimer.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-9
 */

#ifndef __MYTIMER_H__
#define __MYTIMER_H__

#include <bits/types/struct_timeval.h>
#include <cstdint>
#include <cstdio>
#include <sys/time.h>
#include <unistd.h>

namespace hh {

class MyTimer {
public:
  MyTimer();
  explicit MyTimer(int num);
  ~MyTimer();
  void start();
  void stop();
  __int64_t getTime() const;
  void printTime() const;

private:
  int _num;
  bool _flag;
  struct timeval _beginTime;
  struct timeval _endTime;
  __int64_t _totalTimer;
};
} // namespace hh

#endif // __MYTIMER_H