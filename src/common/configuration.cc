/**
 * @file    :configuration.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-11
 */

#include "common/configuration.h"
#include "common/file.h"
#include <boost/locale.hpp>
#include <fstream>
#include <map>
#include <string>

#include <utility>

namespace hh {
Configuration::Configuration(string file_path, Logger &logger)
    : _file_path(std::move(file_path)), _logger(logger) {
  Load();
}

void Configuration::Load() {
  LoadConfig();
  LoadStopWordList();
}

void Configuration::LoadConfig() {
  nlohmann::json mjson;
  std::ifstream jsonfile(_file_path);
  if (!jsonfile.is_open()) {
    LOG_ERROR("open {} fail.");
  }
  try {
    jsonfile >> mjson;
    _connfig = mjson.at("path").get<std::map<std::string, std::string>>();
  } catch (const nlohmann::json::parse_error &e) {
    std::cerr << "JSON parsing error: " << e.what() << std::endl;
  }
}

void Configuration::LoadStopWordList() {
  ifstream stop_word_eng_file = hh::Open(_connfig.at("stop_word_eng").c_str());
  ifstream stop_word_zh_file = hh::Open(_connfig.at("stop_word_zh").c_str());

  string word;
  while (stop_word_eng_file >> word) {
    _stop_word_eng_list.insert(word);
  }
  while (stop_word_zh_file >> word) {
    _stop_word_zh_list.insert(word);
  }
}

map<string, string> &Configuration::GetConfig() { return _connfig; }

set<string> &Configuration::GetStopWordEngList() { return _stop_word_eng_list; }

set<string> &Configuration::GetStopWordZhList() { return _stop_word_zh_list; }

} // namespace hh