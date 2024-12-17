/**
 * @file    :myTimer.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-15
 */

#include "key/myTimer.h"
#include <sys/select.h>

namespace hh {
MyTimer::MyTimer() {
  _flag = false;
  gettimeofday(&_beginTime, nullptr);
}

MyTimer::MyTimer(int num) : _num(num), _totalTimer(0) { _flag = true; }

MyTimer::~MyTimer() {
  if (_flag == false) {
    gettimeofday(&_endTime, nullptr);
    printf("total time = %ld us \n",
           (_endTime.tv_sec - _beginTime.tv_sec) * 1000000 + _endTime.tv_usec -
               _beginTime.tv_usec);
  }
}
void MyTimer::start() { gettimeofday(&_beginTime, nullptr); }

void MyTimer::stop() {
  gettimeofday(&_endTime, nullptr);
  _totalTimer = (_endTime.tv_sec - _beginTime.tv_sec) * 1000000 +
                _endTime.tv_usec - _beginTime.tv_usec;
}

__int64_t MyTimer::getTime() const { return _totalTimer; }

void MyTimer::printTime() const {
  printf("total time = %ld us\n", _totalTimer);
}

} // namespace hh