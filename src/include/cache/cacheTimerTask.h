/**
 * @file    :cacheTimerTask.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#ifndef __CACHETIMERTASK_H__
#define __CACHETIMERTASK_H__

#include "cache/timerTask.h"

namespace hh {
class CacheTimerTask : public TimerTask {
public:
  ~CacheTimerTask() override{};
  void process() override;
};
} // namespace hh

#endif //  CACHETIMERTASK_H