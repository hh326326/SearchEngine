/**
 * @file    :file.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */

#include "common/file.h"
#include <fstream>
#include <stdexcept>

namespace hh {
ifstream Open(const char *path, ios_base::openmode mode) {
  ifstream file(path, mode);
  if (!file) {
    string err("Unable to open file");
    err.append(path);
    //这一行代码是抛出一个 std::runtime_error 类型的异常。std::runtime_error 是
    //C++ 标准库中的一个异常类，表示程序运行时出现的错误。
    throw std::runtime_error(err);
  }
  // file.exceptions(ifstream::badbit); 的作用是设置 file 流在遇到 badbit
  // 错误时抛出异常
  file.exceptions(ifstream::badbit);
  return file;
}
} // namespace hh