/**
 * @file    :spilt_tool.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-13
 */

#ifndef __SPLIT_TOOL_H__
#define __SPLIT_TOOL_H__

#include "common/configuration.h"
#include "cppjieba/Jieba.hpp"
#include "cppjieba/MixSegment.hpp"
#include "key/dictionary.h"
#include "logger.h"
#include <atomic>
#include <cstdint>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace hh {
using std::set;
using std::string;
using std::vector;

class SplitTool {
public:
  explicit SplitTool(Logger &logger, IConfiguration &config);
  void Cut(const string &str, vector<string> &wordsVec, bool flag);
  void Cut(const string &str, unordered_map<string, uint64_t> &wordsMap,
           bool flag);

private:
  void ReadStopWord(set<string> &stopword, const string &stopwordPath);

private:
  Logger &_logger;
  IConfiguration &_config;
  cppjieba::MixSegment _seg;
  cppjieba::Jieba _jieba;
};

} // namespace hh

#endif // SPLIT_TOOL_H