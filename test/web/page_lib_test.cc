#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/configuration.h"
#include <cstddef>

#include "doctest/doctest.h"
#include "web/page_lib.h"

namespace t15 {
using namespace hh;
auto logger = Logger::GetLogger("/home/hh/searchEngine/log/unit_test_15.log");
Configuration config("/home/hh/searchEngine/conf/configure.json");

TEST_CASE("test") {
  config.Load();
  PageLib page_lib( config);
  page_lib.Load();
  CHECK(page_lib._ripe_pages.size() > 0);
  logger->info("page_lib.ripe.pages.size() = {}", page_lib._ripe_pages.size());
  for (size_t i = 0; i < 5; ++i) {
    logger->info("{}", page_lib._ripe_pages[i].c_str());
  }
  // logger.Info("{}", page_lib._ripe_pages[217].c_str());
  page_lib.StoreOnDisk();
}
} // namespace t15