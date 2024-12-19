#include "common/configuration.h"
#include "common/logger.h"

using namespace hh; // NOLINT
int main() {
  auto logger = hh::Logger::GetLogger("./log/unit_test_hh.log");
  hh::Configuration conf("./conf/configure.json");
  logger->warn("hello {}", "word");
  std::map<string, string> mymap = conf.GetConfig();

  for (const auto &elem : mymap) {
    std::cout << elem.first << " <> " << elem.second << std::endl;
  }
  logger->info(LOG_PREFIX("in main: {}"), "done OK"); // 类外这样使用
  //   LOG_INFO("in main2: {}", "done OK"); // 类内这样使用
}