/**
 * @file    :timerManager.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-27
 */

#ifndef __TIMERMANAGER_H__
#define __TIMERMANAGER_H__

#include "cache/timerTask.h"
#include <pthread.h>
#include <vector>

namespace hh {
class TimerManager {
public:
  static TimerManager *GetTimerManager();
  static void Init();
  static void Destory();
  void InitTimer(int delay, int interval);

  void Start();
  void Stop();
  // attach添加 detach分离
  void Attach(TimerTask *);
  void Detach(TimerTask *);

private:
  TimerManager();
  ~TimerManager(){};

  int CreateTimerFd();
  void SetTimerFd(int delay, int interval);
  void handleRead();

  int _timer_fd;
  int _delay;
  int _interval;
  bool _is_started;

  static TimerManager *_timer_manager;
  static pthread_once_t _once;
  std::vector<TimerTask *> _wheelList;
};
} // namespace hh

#endif // TIMERMANAGER_H