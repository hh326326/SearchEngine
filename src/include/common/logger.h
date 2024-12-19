// 这部分是用来禁用特定的编译器警告。在此例中，它禁用了 Clang 编译器的
// -Wgnu-zero-variadic-macro-arguments
// 警告。这个警告通常是在宏使用可变参数时，当没有传递参数时会警告。通过禁用这个警告，可以避免不必要的编译器输出。
#include <cstdint>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

/**
 * @file    :logger.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-13
 */
// 头文件保护
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <string>

// LOG_PREFIX(msg)
// 宏用于生成一个带有文件名、行号和函数名的日志前缀。__FILE__、__LINE__ 和
// __FUNCTION__
// 是预定义的宏，用来获取当前源文件的名字、当前行号和当前函数的名字。这样可以在日志中包含详细的上下文信息，帮助开发者更好地定位问题。
#define LOG_PREFIX(msg)                                                        \
  string("[")                                                                  \
      .append(__FILE__)                                                        \
      .append(":")                                                             \
      .append(std::to_string(__LINE__))                                        \
      .append(":")                                                             \
      .append(__FUNCTION__)                                                    \
      .append("] ")                                                            \
      .append(msg)                                                             \
      .c_str()


#pragma clang diagnostic pop

namespace hh {
using DocId = uint64_t;
using TF_IDF_Weight = double;
using std::string;

// Logger 类提供了用于记录日志的功能。构造函数接收一个日志文件名，并使用 spdlog
// 库的 basic_logger_mt 创建一个多线程支持的日志记录器。

// spdlog::set_pattern 设置日志格式，[%D:%H:%M:%S %z] [%n] [%l] [thread %t] %v
// 代表日志的时间、日志名称、日志级别、线程信息和日志消息内容。
//如果初始化失败，会捕获 spdlog::spdlog_ex 异常并输出错误信息，然后退出程序。
class Logger {
public:
  static std::shared_ptr<spdlog::logger>
  GetLogger(const std::string &filename = "./log/server_test.log") {
    std::lock_guard<std::mutex> lock(m_mutex); // 确保线程安全
    if (!_logger) {
            try {
                spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%l] [thread %t] %v");
                _logger = spdlog::basic_logger_mt("logger", filename); // 创建日志记录器
                _logger->set_level(spdlog::level::info);  // 设置日志级别
            } catch (const spdlog::spdlog_ex &ex) {
                std::cerr << "Log init failed: " << ex.what() << '\n';
                exit(1);  // 初始化失败时终止程序
            }
        }
        return _logger;
  }


private:
  static std::shared_ptr<spdlog::logger> _logger;
      static std::mutex m_mutex;  // 用于线程同步
};

} // namespace hh



#endif // LOGGEr_H