/**
 * @file    :dict_producer.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-14
 */

#include "key/dict_producer.h"
#include "cppjieba/Jieba.hpp"

#include <cctype>
#include <cstddef>
#include <cstdio>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <locale>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace hh {

using std::ifstream;
using std::istringstream;
using std::ostream;
using std::string;

DictProducer::DictProducer(Logger &logger, IConfiguration &conf)
    : _logger(logger), _conf(conf), _st(_logger, _conf) {
  _dict.reserve(100);
}

void DictProducer::BuildEnDict() {
  if (!_dict.empty()) {
    _dict.clear();
  }

  std::map<std::string, int> dictmap; // store word--frequency
  std::set<std::string> stopword;     // store stopword

  const string stopwordpath = _conf.GetConfig()["stop_word_eng"];
  // 读取停用词
  DictProducer::ReadStopWord(stopword, std::move(string(stopwordpath)));
  const string path = _conf.GetConfig()["english"];
  ifstream ifs(path);
  if (!ifs) {
    LOG_ERROR("open {} fail.", path);
  }
  string line;
  while (getline(ifs, line)) {
    PreProcess(line);
    istringstream iss(line);
    string word;
    while (iss >> word) {
      // 不是停用词
      if (stopword.count(word) == 0) {
        auto iter = dictmap.find(word);
        if (iter != dictmap.end()) {
          ++iter->second; // 如果单词在dicmap中，词频++
        } else {
          dictmap[word] = 1;
        }
      }
    }
  }
  _dict.reserve(dictmap.size());
  // 转到vector中
  for (const auto &elem : dictmap) {
    _dict.emplace_back(std::make_pair(elem.first, elem.second));
  }
}
void DictProducer::BuildZhDict() {
  if (!_dict.empty()) {
    _dict.clear();
  }

  std::vector<std::string> filenamevec; // store filenames
  std::map<std::string, int> dictmap;   // store word -- frequency
  std::set<std::string> stopword;       // store stopword

  const string stopwordpath = _conf.GetConfig()["stop_word_zh"];
  // 读取停用词
  DictProducer::ReadStopWord(stopword, std::move(string(stopwordpath)));
  // 循环读取语料文件
  DictProducer::GetFileNames(filenamevec, _conf.GetConfig()["dir_art"]);
  const string path = _conf.GetConfig()["dir_art"];
  for (auto &filename : filenamevec) {
    filename = string(path).append(filename);
    ifstream ifs(filename);
    if (!ifs) {
      LOG_ERROR("open {} fail.", filename);
      return;
    }
    string line;
    while (getline(ifs, line)) {
      _file.push_back(line);
    }
  }

  // 使用cppjieba进行分词
  vector<string> wordvec;
  for (auto &line : _file) {
    _st.Cut(line, wordvec, true);
    // 遍历每一行分词的结果
    for (auto &elem : wordvec) {
      if (IsAlphaNumber(elem) || elem == "一一") {
        continue;
      }
      // 不是停用词
      if (stopword.count(elem) == 0) {
        auto iter = dictmap.find(elem);
        if (iter != dictmap.find(elem)) {
          ++iter->second;
        } else {
          dictmap[elem] = 1;
        }
      }
    }
  }
  _dict.clear();
  _dict.reserve(dictmap.size());
  for (const auto &elem : dictmap) {
    _dict.emplace_back(std::make_pair(elem.first, elem.second));
  }
}

void DictProducer::CreateIndex(Type type) {
  if (!_index.empty()) {
    _index.clear();
  }
  int letter_size;
  if (type == Type::ZH) {
    letter_size = 3;
  } else {
    letter_size = 1;
  }

  for (size_t idx = 0; idx < _dict.size(); ++idx) {

    for (int pos = 0; pos < _dict[idx].first.size(); pos += letter_size) {
      string letter(_dict[idx].first, pos, letter_size);
      _index[letter].insert(static_cast<int>(idx));
    }
  }
}
int DictProducer::Store(Type type) {
  std::ofstream ofs_dict;
  std::ofstream ofs_index;
  string dictfile;
  string indexfile;
  if (type == Type::ZH) {
    dictfile = _conf.GetConfig()["dict_zh"];
    indexfile = _conf.GetConfig()["index_zh"];
  } else {
    dictfile = _conf.GetConfig()["dict_eng"];
    indexfile = _conf.GetConfig()["index_eng"];
  }
  ofs_dict.open(dictfile);
  ofs_index.open(indexfile);
  if (ofs_dict.good() && ofs_index.good()) {
    // store dict
    for (auto &item : _dict) {
      ofs_dict << item.first << "\t" << item.second << '\n';
    }
    // store index
    for (auto &item : _index) {
      ofs_index << item.first << "\t";
      for (const auto &idx : _index[item.first]) {
        ofs_index << " " << idx;
      }
      ofs_index << "\n";
    }
  } else {
    LOG_ERROR("open {} or {} fail.", dictfile, indexfile);
    return -1;
  }
  return 0;
}

// TODO(seven): consider refactor->common/dirsanner
// TODO(seven): consider refactor->common/dirsanner
void DictProducer::GetFileNames(vector<string> &vec, string &path) {
  // 使用 opendir 函数打开指定路径的目录，并获取 DIR 指针
  DIR *ptr_dir = opendir(path.c_str());
  if (ptr_dir == nullptr) {
    // 如果目录无法打开，打印错误日志并返回
    LOG_ERROR("open directory: {} fail.", path);
    return;
  }

  // 定义指向目录项的指针 ptrdirent 和用于存储文件信息的 stat 结构体 ptrstat
  struct dirent *ptrdirent;
  struct stat ptrstat;

  // 使用 readdir 函数读取目录中的每个条目，直到所有条目被读取完
  while ((ptrdirent = readdir(ptr_dir)) != nullptr) {
    // 使用 stat 函数获取目录项的信息，存入 ptrstat 结构体（虽然目前没有使用它）
    int ret = stat(ptrdirent->d_name, &ptrstat);

    // 创建一个 string 类型的变量 filename，存储当前读取到的目录项名称
    string filename(ptrdirent->d_name);

    // 忽略当前目录 "." 和上级目录 ".."
    if (filename != "." && filename != "..") {
      // 将有效的文件名（不为 "." 和 ".."）添加到传入的 vector 容器 vec 中
      vec.emplace_back(filename);
    }
  }

  // 关闭目录流，释放资源
  closedir(ptr_dir);
}

// TODO(seven): consider refactor->common/configuration
void DictProducer::ReadStopWord(set<string> &stopword,
                                const string &stopwordpath) {
  // 读取停用词
  ifstream ifs(stopwordpath);
  if (!ifs) {
    LOG_ERROR("open {} fail.", stopwordpath);
    return;
  }
  string line;
  while (ifs >> line) {
    stopword.insert(line);
  }
  ifs.close();
}

void DictProducer::PreProcess(string &word) {
  for (auto &elem : word) {
    if (isalpha(elem)) {
      // 是字母，转为小写
      elem = static_cast<char>(tolower(elem));
    } else {
      // 其他字符，转为空格
      elem = ' ';
    }
  }
}

bool DictProducer::IsAlphaNumber(string &word) {
  char firstByte = word.c_str()[0];
  bool flag = true;
  if ((firstByte & 0x80) == 0x0) {
    flag = true;
  } else if ((firstByte & 0xE0) == 0xE0) {
    flag = false;
  }
  return flag;
}
size_t DictProducer::GetIndex(string &word) {
  size_t idx;
  for (idx = 0; idx < _dict.size(); ++idx) {
    if (_dict[idx].first == word) {
      return idx;
    }
  }
  return _dict.size() + 1;
}

} // namespace hh