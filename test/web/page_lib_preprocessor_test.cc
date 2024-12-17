#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include "web/page_lib_preprocessor.h"

namespace t29 {
using namespace hh; // NOLINT
hh::Logger logger("/home/hh/searchEngine/log/unit_test_29_2.log");

Configuration config("/home/hh/searchEngine/conf/configure.json", logger);

TEST_CASE("test ") {
  logger.SetLogLevel(spdlog::level::debug);
  PageLib page_lib(logger, config);
  page_lib.Load();
  REQUIRE(page_lib.GetRipePages().size() > 0);
  PageLibPreprocessor page_lib_preprocessor(page_lib, logger, config);
  page_lib_preprocessor.CreatePageLibList();
  logger.Info("before size: {}", page_lib_preprocessor.GetPageLibList().size());
  page_lib_preprocessor.RmRedundantPages();
  logger.Info("after size: {}", page_lib_preprocessor.GetPageLibList().size());
  page_lib_preprocessor.Cut();
  // for (const auto& pair : page_lib_preprocessor._total_term_frequencies){
  //   logger.Info("{} {}", pair.first, pair.second);
  // }
  logger.Info("-------------------------");
  logger.Info("-------------------------");
  logger.Info("-------------------------");
  page_lib_preprocessor.BuildInvertIndexTable();
  for (const auto &pair : page_lib_preprocessor._invert_index_table) {
    auto set = pair.second;
    logger.Info("--------word: {}", pair.first);
    for (const auto &pair : set) {
      logger.Info("{} {}", pair.first, pair.second);
    }
  }
  page_lib_preprocessor.StoreOnDisk();
}

} // namespace t29