/**
 * @file    :dict_producer.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-14
 */

#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include <cstddef>
#include <dirent.h>
#include <fstream>
#include <set>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include "common/configuration.h"
#include "common/logger.h"
#include "common/split_tool.h"
#include "spdlog/spdlog.h"

namespace hh {
using std::ifstream;
using std::string;
using std::vector;
// using Type = enum{ZH, ENG};
using Type = enum class Language { ZH, ENG };

class DictProducer {
public:
  explicit DictProducer( IConfiguration &conf);
  void BuildEnDict();
  void BuildZhDict();
  void CreateIndex(Type type);
  int Store(Type type);
  // TODO(seven): consider refactor->common/dirsanner
  void GetFileNames(vector<string> &vec, string &path);
  // TODO(seven): consider refactor->common/configuration
  void ReadStopWord(set<string> &stopword, const string &stopwordpath);

private:
  static void PreProcess(string &word);
  static bool IsAlphaNumber(string &word);
  size_t GetIndex(string &word);

private:
  IConfiguration &_conf;
  SplitTool _st;
  std::vector<std::string> _file;
  std::vector<std::pair<std::string, int>> _dict;
  std::map<std::string, std::set<int>> _index;
};

} // namespace hh

#endif // DICTPRODUCER_H