#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include "web/web_page_query.h"

namespace t2x {
using namespace hh; // NOLINT
hh::Logger logger("/home/hh/searchEngine/log/unit_test_2x_4.log");
Configuration config("/home/hh/searchEngine/conf/configure.json", logger);
TEST_CASE("test") {
  logger.SetLogLevel(spdlog::level::debug);
  PageLib page_lib(logger, config);
  page_lib.Load();
  REQUIRE(page_lib.GetRipePages().size() > 0);
  PageLibPreprocessor page_lib_preprocessor(page_lib, logger, config);
  page_lib_preprocessor.CreatePageLibList();
  page_lib_preprocessor.RmRedundantPages();
  page_lib_preprocessor.Cut();
  page_lib_preprocessor.BuildInvertIndexTable();
  page_lib_preprocessor.LoadIndexPageList();

  WebPageQuery query(page_lib_preprocessor, logger, config);

  SUBCASE("test cut") {
    auto result = query.Cut("我是一个好人");
    for (const auto &pair : result) {
      logger.Debug("{} {}", pair.first, pair.second);
    }
    CHECK(result.size() > 0);
    auto search = result.find("好人");
    CHECK(search != result.end());
  }

  SUBCASE("test cut") {
    auto result = query.Cut("我要卖车！");
    for (const auto &pair : result) {
      logger.Debug("{} {}", pair.first, pair.second);
    }
    CHECK(result.size() > 0);
    auto search = result.find("卖车");
    CHECK(search != result.end());
    auto pages_id = query.GetRelativePages(result);
    for (auto page : pages_id) {
      logger.Info("page id: {}", page);
    }
  }

  SUBCASE("test GetRelativePages") {
    // auto result = query.Cut("中国移动通信集团公司"); //! don't split word
    auto result = query.Cut("原油生产平稳");
    // for (auto& pair : result) {
    //   logger.Debug("{} {}", pair.first, pair.second);
    // }
    CHECK(result.size() > 0);
    auto search = result.find("原油");
    CHECK(search != result.end());
    auto pages_id = query.GetRelativePages(result);
    for (auto page : pages_id) {
      logger.Debug("page id: {}", page);
    }
  }

  SUBCASE("test caculate value") {
    // auto result = query.Cut("中国移动通信集团公司"); //! don't split word
    auto result = query.Cut("原油生产平稳");
    for (auto &pair : result) {
      logger.Debug("{} {}", pair.first, pair.second);
    }
    REQUIRE(result.size() > 0);
    auto search = result.find("原油");
    REQUIRE(search != result.end());

    auto relative_pages_vectors = query.CalculateRelativePagesVectors(result);
    CHECK(relative_pages_vectors.size() > 0);
    for (const auto &pair : relative_pages_vectors) {
      logger.Debug("-----------{}", pair.first);
      for (const auto &map_pair : pair.second) {
        logger.Debug("{} {}", map_pair.first, map_pair.second);
      }
    }
  }

  //   SUBCASE("test do query") {
  //     auto result = query.DoQuery("我");
  //     logger.Debug("{}", result);

  //     result = query.DoQuery("石油平稳生产");
  //     logger.Debug("{}", result);
  //   }
}

} // namespace t2x