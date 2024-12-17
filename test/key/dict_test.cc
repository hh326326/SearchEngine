#include "common/configuration.h"
#include "common/logger.h"
#include "key/dict_producer.h"

using namespace hh; // NOLINT
int main() {
  hh::Logger logger("/home/hh/searchEngine/log/unit_test_hh.log");
  hh::Configuration config("/home/hh/searchEngine/conf/configure.json", logger);

  std::cout << config.GetConfig()["dir_art"] << std::endl;
  logger.Info(LOG_PREFIX("in main 333 {}"), "ok");
  DictProducer dp(logger, config);

  dp.BuildEnDict();
  dp.CreateIndex(Type::ENG);
  dp.Store(Type::ENG);

  dp.BuildZhDict();
  dp.CreateIndex(Type::ZH);
  dp.Store(Type::ZH);

  std::cout << "main done" << std::endl;
}