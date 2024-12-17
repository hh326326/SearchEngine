#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "common/configuration.h"
#include "doctest/doctest.h"
#include <iostream>

namespace t1 {
hh::Logger logger("/home/hh/searchEngine/log/unit_test_empty.log");
TEST_CASE("Configuration Load internal") {
  const char *test_file_path = "/home/hh/searchEngine/conf/configure.json";
  hh::Configuration config{test_file_path, logger};

  SUBCASE("test LoadConfig() and test LoadStopWordList()") {
    config.LoadConfig();
    auto &config_map = config.GetConfig();
    // for (const auto& [key, value] : config_map) {
    //   std::cout << key << ":" << value << std::endl;
    //   logger.Info("{}:{}", key, value);
    // }
    CHECK(config_map.at("stop_word_eng") != "wrong_path");
    CHECK(config_map.at("stop_word_eng") ==
          "/home/hh/searchEngine/data/raw_data/yuliao/stop_words_eng.txt");
    CHECK(config_map.at("stop_word_zh") ==
          "/home/hh/searchEngine/data/raw_data/yuliao/stop_words_zh.txt");
    CHECK(config_map.at("user.dict") ==
          "/home/hh/searchEngine/data/dict/user.dict.utf8");
    CHECK(config_map.at("dir_art") ==
          "/home/hh/searchEngine/data/raw_data/yuliao/art/");
    config.LoadStopWordList();
    auto stop_word_eng_list = config.GetStopWordEngList();
    auto stop_word_zh_list = config.GetStopWordZhList();
    auto search = stop_word_eng_list.find("simpleNO1");
    CHECK(search == stop_word_eng_list.end());
    search = stop_word_eng_list.find("won");
    CHECK(search != stop_word_eng_list.end());
    search = stop_word_zh_list.find("嗳");
    CHECK(search != stop_word_zh_list.end());
    search = stop_word_zh_list.find("在");
    CHECK(search != stop_word_zh_list.end());
  }
  SUBCASE("test Load()") {
    config.Load();
    auto &config_map = config.GetConfig();

    CHECK(config_map.at("stop_word_eng") != "wrong_path");
    CHECK(config_map.at("stop_word_eng") ==
          "/home/hh/searchEngine/data/raw_data/yuliao/stop_words_eng.txt");
    CHECK(config_map.at("stop_word_zh") ==
          "/home/hh/searchEngine/data/raw_data/yuliao/stop_words_zh.txt");
    CHECK(config_map.at("user.dict") ==
          "/home/hh/searchEngine/data/dict/user.dict.utf8");
    CHECK(config_map.at("dir_art") ==
          "/home/hh/searchEngine/data/raw_data/yuliao/art/");
    auto stop_word_eng_list = config.GetStopWordEngList();
    auto stop_word_zh_list = config.GetStopWordZhList();
    auto search = stop_word_eng_list.find("simpleNO1");

    for (const auto &word : stop_word_zh_list) {
      std::cout << word << '\n'; // 打印所有中文停用词
    }
    std::cout << '\n';

    CHECK(search == stop_word_eng_list.end());
    search = stop_word_eng_list.find("won");
    CHECK(search != stop_word_eng_list.end());
    search = stop_word_zh_list.find("嗳");
    CHECK(search != stop_word_zh_list.end());
    // std::cout << *search << '\n';
    search = stop_word_zh_list.find("在");
    // std::cout << *search << '\n';
    CHECK(search != stop_word_zh_list.end());
  }
}
} // namespace t1