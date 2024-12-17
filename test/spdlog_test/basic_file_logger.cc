#include "spdlog/sinks/basic_file_sink.h"
#include <filesystem> // C++17 中支持
#include <iostream>
#include <spdlog/spdlog.h> // 必须包含此头文件

using namespace std;

void basic_logfile_example() {
  try {
    // 检查并创建目录（如果不存在）
    // if (!filesystem::exists("test")) {
    // filesystem::create_directory("test");
    // }

    // 创建日志记录器，日志输出到 "test/basic-log.txt"
    auto logger = spdlog::basic_logger_mt("basic_logger", "test/basic-log.txt");

    // 设置日志级别（可选）
    logger->set_level(spdlog::level::info);

    // 示例日志输出
    logger->info("This is an info log message.");
    logger->warn("This is a warning message.");
    logger->error("This is an error message.");
  } catch (const spdlog::spdlog_ex &ex) {
    cout << "Log init failed: " << ex.what() << '\n';
  }
}

int main() {
  basic_logfile_example();
  return 0;
}
