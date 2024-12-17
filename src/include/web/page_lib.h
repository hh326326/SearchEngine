/**
 * @file    :page_lib.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-14
 */

#ifndef __PAGE_LIB_H__
#define __PAGE_LIB_H__

#include "common/configuration.h"
#include "common/dir_scanner.h"
#include "web/rss_parser.h"
#include "web/web_page.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hh {
using simhash::Simhasher;
using std::pair;
using std::unordered_map;

// [[nodiscard]]是一个属性说明符（attribute specifier），用于 C++ 17
// 及以上版本。它向编译器和其他程序员传达一个重要的信息：函数的返回值不应该被忽略。
class PageLib {
public:
  PageLib(Logger &logger, IConfiguration &config);
  // 从文件中加载网页库
  void Load();
  // 获取网页库中的网页数量
  [[nodiscard]] size_t GetPageNum() const;
  vector<RssItem> &GetRssItems();
  vector<string> &GetRipePages();
  Simhasher &GetSimhaser();
  // 离线存储网页库 'ripe_page.dat'和位置偏移库offset_lib.dat'
  void StoreOnDisk();

  // private:
  Logger &_logger;
  IConfiguration &_config;
  DirScanner _dir_scanner;
  // 解析单个rss文件
  RssParser _rss_parser;
  simhash::Simhasher _simhasher;
  // 存放每篇文档的网页库的位置信息
  unordered_map<uint64_t, pair<uint64_t, uint64_t>> _offset_lib;
  // 存放格式化之后的网页
  vector<string> _ripe_pages;
};

} // namespace hh

#endif // PAGE_LIB_H