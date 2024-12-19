/**
 * @file    :dictionary.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-7
 */

#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "common/configuration.h"
#include "common/logger.h"
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hh {
using std::ifstream;
using std::istringstream;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

class Dictionary {
public:
  Dictionary(
             IConfiguration
                 &config) noexcept; // noexcept 声明构造函数 不会抛出任何异常
  unordered_map<int, int> DoQuery(const string &key);
  vector<pair<string, int>> &
  GetEnDict(); // 英文词典，string --> 单词 int --> 出现次数
  vector<pair<string, int>> &GetZhDict(); // 中文词典
  unordered_map<string, set<int>> &
  GetEnIndex(); // 英文索引 字母 --> 有此字母的单词的集合 的索引
  unordered_map<string, set<int>> &GetZhIndex(); // ...
  static int CharLen(const string &c);

private:
  int ReadDict();
  int ReadIndex();

private:
  IConfiguration &_config;
  vector<pair<string, int>> _dict_en;        // 读取英文词典
  vector<pair<string, int>> _dict_zh;        // 读取中文词典
  unordered_map<string, set<int>> _index_en; // 读取英文索引
  unordered_map<string, set<int>> _index_zh; // ...
};

} // namespace hh

#endif // DICTIONARY_H