/**
 * @file    :dir_scanner.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-14
 */

#ifndef __DIR_SCANNER_H__
#define __DIR_SCANNER_H__

#include "configuration.h"
#include "logger.h"
#include <filesystem>
#include <string>
#include <vector>

namespace hh {
using std::string;
using std::vector;

/// Recursively scan a directory and return a vector of all files in it
///@todo Make config para const, this neew to find passaround to make
/// operator()() const

class DirScanner {
public:
  explicit DirScanner( IConfiguration &config);
  // 调用Traverse()函数, 扫描指定目录下的所有文件
  void operator()();
  vector<string> &GetFilesPath();
  // 获取目标文件夹下的所有文件
  void Traverse(const string &dir_path);

private:
  vector<string> _file_path_list; // 存放每个语料文件的绝对路径
  IConfiguration &_config;
};

} // namespace hh

#endif // DIR_SCANNER_H