/**
 * @file    :task.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __TASK_H__
#define __TASK_H__

#include <functional>

namespace hh {
using std::function;

using Task = function<void()>;
} // namespace hh
#endif
