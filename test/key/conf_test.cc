#include "common/configuration.h"
#include "common/logger.h"

using namespace hh; // NOLINT
int main() {

  hh::Logger logger("/home/hh/searchEngine/log/unit_test_hh.log");
  hh::Configuration conf("/home/hh/searchEngine/conf/configure.json", logger);
  logger.Warn("hello {}", "word");
  std::map<string, string> mymap = conf.GetConfig();

  for (const auto &elem : mymap) {
    std::cout << elem.first << " <> " << elem.second << std::endl;
  }
  logger.Info(LOG_PREFIX("in main: {}"), "done OK"); // 类外这样使用
  //   LOG_INFO("in main2: {}", "done OK"); // 类内这样使用
}