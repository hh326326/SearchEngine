/**
 * @file    :config_one.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-18
 */

#ifndef __CONFIGONE_H__
#define __CONFIGONE_H__

#include "file.h"
#include "logger.h"
#include "nlohmann/json.hpp"
#include <map>
#include <set>
#include <string>
#include <utility>

// auto &config = *Configuration::getInstance()
// config["server_ip"] = 127.0.0.1
//配置文件格式：server_ip 127.0.0.1
//可随意添加

namespace hh {
using std::map;
using std::set;
using std::string;

class ConfigOne {

  ConfigOne(const ConfigOne &) = delete;
  ConfigOne &operator=(const ConfigOne &) = delete;
  ConfigOne(ConfigOne &&) = default;
  ConfigOne &operator=(ConfigOne &&) = delete;

  public:
  static ConfigOne *getConfigInstance();
  void Load();
  static void init();
  static void destory();
  // const std::string &operator[](const std::string &key);
  map<string, string> &GetConfig();
  set<string> &GetStopWordEngList();
  set<string> &GetStopWordZhList();

private:
  ConfigOne(const string file_path);
  ~ConfigOne() = default;
  void LoadConfig();
  void LoadStopWordList();

private:
  map<string, string> _config;
  set<string> _stop_word_eng_list;
  set<string> _stop_word_zh_list;
  // static string _file_path;
  string _file_path;
  static ConfigOne *_pConfig;
  static pthread_once_t _once;
};

} // namespace hh

#endif // CONFIGONE_H