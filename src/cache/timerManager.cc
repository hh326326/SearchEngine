/**
 * @file    :timerManager.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#include "cache/timerManager.h"
#include "cache/cacheTimerTask.h"
#include "cache/timerTask.h"
#include "common/config_one.h"
#include <algorithm>
#include <asm-generic/errno-base.h>
#include <bits/types/struct_itimerspec.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <poll.h>
#include <pthread.h>
#include <string>
#include <sys/poll.h>
#include <sys/timerfd.h>
#include <unistd.h>

namespace hh {
TimerManager *TimerManager::_timer_manager = nullptr;
pthread_once_t TimerManager::_once = PTHREAD_ONCE_INIT;

TimerManager *TimerManager::GetTimerManager() {
  pthread_once(&_once, Init);
  return _timer_manager;
}

void TimerManager::Init() {
  _timer_manager = new TimerManager();
  atexit(Destory);
}

void TimerManager::Destory() {
  if (_timer_manager) {
    delete _timer_manager;
    _timer_manager = nullptr;
  }
}

TimerManager::TimerManager() : _timer_fd(CreateTimerFd()), _is_started(false) {
  // config
  map<string, string> config_map = ConfigOne::getConfigInstance()->GetConfig();
  InitTimer(std::atoi(config_map["timer_delay"].c_str()),
            std::atoi(config_map["timer_interval"].c_str()));
  // InitTimer(5, 10);
}

void TimerManager::InitTimer(int delay, int interval) {
  _delay = delay;
  _interval = interval;
}

void TimerManager::Start() {
  struct pollfd pfd;
  pfd.fd = _timer_fd;
  pfd.events = POLLIN;

  CacheTimerTask task;
  _timer_manager->Attach(&task);

  SetTimerFd(_delay, _interval);

  _is_started = true;
  while (_is_started) {
    int nready = poll(&pfd, 1, 5000);
    if (-1 == nready && errno == EINTR) {
      continue;
    } else if (-1 == nready) {
      perror("-1 == nready");
      return;
    } else if (0 == nready) {
      continue;
    } else {
      if (pfd.events & POLLIN) {
        // 先处理读事件
        handleRead();
        // 再去执行任务
        for (auto &task : _wheelList) {
          task->process();
        }
      }
    }
  }
  return;
}

void TimerManager::Stop() {
  _is_started = false;
  SetTimerFd(0, 0); // 关闭定时器
  return;
}

void TimerManager::Attach(TimerTask *task) {
  auto it = std::find(_wheelList.begin(), _wheelList.end(), task);
  if (it == _wheelList.end()) {
    _wheelList.push_back(task);
  }
  return;
}

void TimerManager::Detach(TimerTask *task) {
  auto it = std::find(_wheelList.begin(), _wheelList.end(), task);
  if (it != _wheelList.end()) {
    _wheelList.erase(it);
  }
  return;
}

int TimerManager::CreateTimerFd() {
  int fd = timerfd_create(CLOCK_REALTIME, 0);
  if (-1 == fd) {
    perror("timerfd_create");
    return -1;
  }
  return fd;
}

void TimerManager::SetTimerFd(int delay, int interval) {
  struct itimerspec value;
  value.it_value.tv_sec = delay;
  value.it_value.tv_nsec = 0;
  value.it_interval.tv_sec = interval;
  value.it_interval.tv_nsec = 0;
  int ret = timerfd_settime(_timer_fd, 0, &value, nullptr);
  if (-1 == ret) {
    perror("timerfd_settime");
  }
}

void TimerManager::handleRead() {
  uint64_t value;
  int ret = read(_timer_fd, &value, sizeof(value));
  if (ret != sizeof(value)) {
    perror("read");
    return;
  }
}

} // namespace hh