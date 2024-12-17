
/**
 * @file    :dir_scanner.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-11
 */

#include "common/dir_scanner.h"

namespace hh {
using std::filesystem::directory_iterator;
using std::filesystem::path;

DirScanner::DirScanner(Logger &logger, IConfiguration &config)
    : _logger(logger), _config(config) {}

// 调用Traverse()函数, 扫描指定目录下的所有文件
void DirScanner::operator()() {
  LOG_INFO("{}", _config.GetConfig()["dir_people"]);
  return Traverse(_config.GetConfig()["dir_people"]);
}

vector<string> &DirScanner::GetFilesPath() { return _file_path_list; }

// 获取目标文件夹下的所有文件
void DirScanner::Traverse(const string &dir_path) {
  const path test_dir_path = {dir_path};
  if (!exists(test_dir_path)) {
    LOG_ERROR("Directory not exist: {}", dir_path);
    return;
  }
  for (const auto &entry : directory_iterator(test_dir_path)) {
    _file_path_list.emplace_back(entry.path().string());
  }
}

} // namespace hh
