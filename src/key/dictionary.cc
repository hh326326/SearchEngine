/**
 * @file    :dictionary.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-13
 */

#include "key/dictionary.h"
#include "cppjieba/Unicode.hpp"
#include <cstddef>
#include <fstream>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>
#include <unordered_map>
#include <utility>

namespace hh {

Dictionary::Dictionary(Logger &logger, IConfiguration &config) noexcept
    : _logger(logger), _config(config) {
  // std::cout << "读取词典，索引文件" << '\n';
  ReadDict();  // 读取词典文件
  ReadIndex(); // 读取索引文件
}

unordered_map<int, int> Dictionary::DoQuery(const string &key) {
  int char_len = CharLen(key);
  unordered_map<int, int> line_freq_map; // 行号，频数

  // 存放单词(词语) 分成的字母(字)
  set<string> tmp_set;
  // 遍历单词(词语)
  for (size_t i = 0; i < key.size(); ++i) {

    string ch = string(key, i, char_len);
    // std::cout << "ch1 = " << ch << '\n';
    tmp_set.insert(ch);
  }
  // 统计频数
  for (const auto &ch : tmp_set) {
    // 注: = 3表示是UTF8是3字节编码, 认为是中文
    // std::cout << "ch2 =  " << ch << '\n';
    if (CharLen(ch) == 3) {
      for (const auto &elem : _index_zh[ch]) {
        ++line_freq_map[elem];
      }
    } else if (CharLen(ch) == 1) {
      // 注: = 3表示是UTF8是1字节编码, 认为是英文
      for (const auto &elem : _index_en[ch]) {
        ++line_freq_map[elem];
      }
    }
  }
  return line_freq_map;
}

// 英文词典，string --> 单词 int --> 出现次数
vector<pair<string, int>> &Dictionary::GetEnDict() { return _dict_en; }

// 中文词典
vector<pair<string, int>> &Dictionary::GetZhDict() { return _dict_zh; }

// 英文索引 字母 --> 有此字母的单词的集合 的索引
unordered_map<string, set<int>> &Dictionary::GetEnIndex() { return _index_en; }

unordered_map<string, set<int>> &Dictionary::GetZhIndex() { return _index_zh; }

int Dictionary::ReadDict() {
  vector<string> fileNameVec = {_config.GetConfig()["dict_eng"],
                                _config.GetConfig()["dict_zh"]};

  for (const auto &filename : fileNameVec) {
    ifstream ifs(filename);
    if (!ifs) {
      LOG_ERROR("open {} fail.", filename);
      return -1;
    }
    string line;
    string word;
    int freq;
    while (getline(ifs, line)) {
      istringstream iss(line);
      iss >> word >> freq;

      // 读取词典
      if (filename == _config.GetConfig()["dict_eng"]) {
        _dict_en.emplace_back(std::make_pair(word, freq));
        //   std::cout << "word = " << word << '\n';
      } else {
        //   std::cout << "word = " << word << '\n';
        _dict_zh.emplace_back(std::make_pair(word, freq));
      }
    }
    ifs.close();
  }
  return 0;
}

int Dictionary::ReadIndex() {
  vector<string> fileNameVec = {_config.GetConfig()["index_eng"],
                                _config.GetConfig()["index_zh"]};
  for (const auto &filename : fileNameVec) {
    ifstream ifs(filename);
    if (!ifs) {
      LOG_ERROR("open {} fail.", filename);
      return -1;
    }
    // std::cout << "ReadIndex()" << '\n';
    string line;
    string ch;
    while (getline(ifs, line)) {
      istringstream iss(line);
      iss >> ch;
      int index;
      set<int> index_set;
      while (iss >> index) {
        index_set.insert(index);
      }
      // 索引存入内存
      if (filename == _config.GetConfig()["index_eng"]) {
        // std::cout << "ch3 = " << ch << '\n';
        _index_en.insert(std::make_pair(ch, index_set));
      } else {
        _index_zh.insert(std::make_pair(ch, index_set));
      }
    }
    ifs.close();
  }
  return 0;
}

// 区分中英文字符
int Dictionary::CharLen(const string &c) {
  char firstByte = c.c_str()[0];
  int ret = -1;
  if ((firstByte & 0x80) == 0x0) {
    ret = 1;
  } else if ((firstByte & 0xE0) == 0xE0) {
    ret = 3;
  }
  return ret;
}

} // namespace hh