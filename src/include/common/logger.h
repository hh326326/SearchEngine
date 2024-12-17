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

// 这些宏允许通过 _logger 对象记录不同级别的日志。它们会调用 Logger 类中的
// Error、Warn、Info 和 Debug 方法来处理不同级别的日志消息。
#define LOG_ERROR(msg, ...) _logger.Error(LOG_PREFIX(msg), ##__VA_ARGS__)
#define LOG_WARN(msg, ...) _logger.Warn(LOG_PREFIX(msg), ##__VA_ARGS__)
#define LOG_INFO(msg, ...) _logger.Info(LOG_PREFIX(msg), ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) _logger.Debug(LOG_PREFIX(msg), ##__VA_ARGS__)

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
  explicit Logger(const std::string &filename) {
    try {
      spdlog::set_pattern("[%D:%H:%M:%S %z] [%n] [%l] [thread %t] %v");
      _logger = spdlog::basic_logger_mt("hh", filename);
      // std::cout << "Logger()" << '\n';
      _logger->error("我在Logger构造函数");
    } catch (const spdlog::spdlog_ex &ex) {
      std::cerr << "Log init failed: " << ex.what() << '\n';
      exit(1);
    }
  }

  //宏（LOG_ERROR, LOG_INFO 等）
  //用来简化日志调用，自动为你生成日志前缀，并将日志传递给模板函数。
  //模板函数（Info, Debug, Warn, Error）
  //是具体的日志记录实现，负责将格式化后的消息传递给 spdlog 库进行输出。

  template <typename... Args> void Info(const std::string &msg, Args... args) {
    _logger->info(msg, args...);
  }

  template <typename... Args> void Debug(const std::string &msg, Args... args) {
    _logger->debug(msg, args...);
  }

  template <typename... Args> void Warn(const std::string &msg, Args... args) {
    _logger->warn(msg, args...);
  }

  template <typename... Args> void Error(const std::string &msg, Args... args) {
    _logger->error(msg, args...);
  }

  //该方法用于设置日志记录的级别，spdlog 支持多种日志级别（例如：trace, debug,
  //info, warn, error 等）。通过调用 SetLogLevel
  //方法，可以动态设置日志的记录级别，控制输出的日志的详细程度。
  void SetLogLevel(spdlog::level::level_enum level) {
    _logger->set_level(level);
  }

  std::shared_ptr<spdlog::logger> getLogger() const { return _logger; }

private:
  std::shared_ptr<spdlog::logger> _logger;
};

} // namespace hh

#endif // LOGGEr_H