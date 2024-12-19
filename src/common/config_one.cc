/**
 * @file    :config_one.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-11
 */

#include "common/config_one.h"
#include "common/file.h"
#include <boost/locale.hpp>
#include <fstream>
#include <map>
#include <string>

#include <utility>

namespace hh {
ConfigOne::ConfigOne(const string file_path)
  : _file_path(file_path)
{
  Load();
}

void ConfigOne::Load() {
  LoadConfig();
//   LoadStopWordList();
}

void ConfigOne::LoadConfig() {
  nlohmann::json mjson;
  std::ifstream jsonfile(_file_path);
  // std::cout << "_file_path： " << _file_path << '\n';
  if (!jsonfile.is_open()) {
    // LOG_ERROR("open {} fail.");
  }
  try {
    jsonfile >> mjson;
    _config = mjson.at("path").get<std::map<std::string, std::string>>();
  } catch (const nlohmann::json::parse_error &e) {
    std::cerr << "JSON parsing error: " << e.what() << std::endl;
  }
  
}

void ConfigOne::LoadStopWordList() {
  ifstream stop_word_eng_file = hh::Open(_config.at("stop_word_eng").c_str());
  ifstream stop_word_zh_file = hh::Open(_config.at("stop_word_zh").c_str());

  string word;
  while (stop_word_eng_file >> word) {
    _stop_word_eng_list.insert(word);
  }
  while (stop_word_zh_file >> word) {
    _stop_word_zh_list.insert(word);
  }
}

ConfigOne *ConfigOne::getConfigInstance()
{
    pthread_once(&_once, init);
    return _pConfig;
}

void ConfigOne::init()
{
    _pConfig = new ConfigOne("/home/hh/searchEngine/conf/configure.json");
    atexit(destory);
}

void ConfigOne::destory()
{
    if(_pConfig)
    {
        delete _pConfig;
        _pConfig = nullptr;
    }
}

// const std::string &ConfigOne::operator[](const std::string &key)
// {
//     return _config.at(key);
// }



map<string, string> &ConfigOne::GetConfig() { return _config; }

set<string> &ConfigOne::GetStopWordEngList() { return _stop_word_eng_list; }

set<string> &ConfigOne::GetStopWordZhList() { return _stop_word_zh_list; }

ConfigOne *ConfigOne::_pConfig = nullptr;
pthread_once_t ConfigOne::_once = PTHREAD_ONCE_INIT;
// std::string ConfigOne::_file_path = "/home/hh/searchEngine/conf/configure.json";


} // namespace hh