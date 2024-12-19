#include "common/configuration.h"
#include "common/logger.h"
#include "key/dict_producer.h"

using namespace hh; // NOLINT
int main() {
  auto logger = hh::Logger::GetLogger("./log/unit_test_hh.log");
  hh::Configuration config("./conf/configure.json");

  std::cout << config.GetConfig()["dir_art"] << std::endl;
  logger->info(LOG_PREFIX("in main 333 {}"), "ok");
  DictProducer dp(config);

  dp.BuildEnDict();
  dp.CreateIndex(Type::ENG);
  dp.Store(Type::ENG);

  dp.BuildZhDict();
  dp.CreateIndex(Type::ZH);
  dp.Store(Type::ZH);

  std::cout << "main done" << std::endl;
}