/**
 * @file    :configuration.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-13
 */

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "file.h"
#include "logger.h"
#include "nlohmann/json.hpp"
#include <map>
#include <set>
#include <string>
#include <utility>

namespace hh {
using std::map;
using std::set;
using std::string;

// 接口类
class IConfiguration {
public:
  IConfiguration() = default;
  virtual ~IConfiguration() = default;
  virtual void Load() = 0;
  virtual void LoadConfig() = 0;
  virtual void LoadStopWordList() = 0;
  virtual map<string, string> &GetConfig() = 0;
  virtual set<string> &GetStopWordEngList() = 0;
  virtual set<string> &GetStopWordZhList() = 0;
};

class Configuration : public IConfiguration {
public:
  explicit Configuration(string file_path);
  ~Configuration() override = default;
  Configuration(const Configuration &) = delete;
  Configuration &operator=(const Configuration &) = delete;
  Configuration(Configuration &&) = default;
  Configuration &operator=(Configuration &&) = delete;

  void Load() override;
  map<string, string> &GetConfig() override;
  set<string> &GetStopWordEngList() override;
  set<string> &GetStopWordZhList() override;

  // private:

  void LoadConfig() override;
  void LoadStopWordList() override;

private:
  map<string, string> _connfig;
  set<string> _stop_word_eng_list;
  set<string> _stop_word_zh_list;
  string _file_path;
};

} // namespace hh

#endif // CONFIGURATION_H