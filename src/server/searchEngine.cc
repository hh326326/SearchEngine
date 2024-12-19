/**
 * @file    :searchEngine.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-19
 */

#include "cache/redis.h"
#include "key/dictionary.h"
#include "key/key_query.h"
#include "reactor/search_engine.h"
#include <memory>
#include <spdlog/logger.h>


using namespace hh;

int main() {
  Configuration config("./conf/configure.json");

  hh::SettingManager manager(config);
  hh::Setting setting = manager.createSetting();
  auto logger = Logger::GetLogger();
  logger->error("I am in main()");

  WebServer server(atoi(config.GetConfig()["threadNum"].c_str()), atoi(config.GetConfig()["queSize"].c_str()), config.GetConfig()["ip"].c_str(), atoi(config.GetConfig()["port"].c_str()),  config, setting);
  server.Start();
}
