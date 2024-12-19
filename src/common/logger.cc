/**
 * @file    :logger.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-19
 */

#include "common/logger.h"


namespace hh {
std::shared_ptr<spdlog::logger> Logger::_logger = nullptr;
std::mutex Logger::m_mutex; // 初始化互斥锁
                            // 
}// namespace hh