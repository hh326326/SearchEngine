/**
 * @file    :timerTask.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-27
 */

#ifndef __TIMERTASK_H__
#define __TIMERTASK_H__

namespace hh {
class TimerTask {
public:
  virtual ~TimerTask() {};
  virtual void process() = 0;
};

} // namespace hh

#endif // TIMERTASK_H