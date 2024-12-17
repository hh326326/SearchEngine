/**
 * @file    :webEngine.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-19
 */

#include "cache/redis.h"
#include "key/dictionary.h"
#include "reactor/web_server.h"

using namespace hh;

int main() {
  hh::Logger logger("/home/hh/searchEngine/log/server_test.log");
  Configuration config("/home/hh/searchEngine/conf/configure.json", logger);
  RedisCache cache(logger, config);
  Dictionary dict(logger, config);
  KeyRecommander keyRecom(dict);

  PageLib pl(logger, config);
  PageLibPreprocessor plp(pl, logger, config);

  auto _logger = logger.getLogger(); // 强制刷新
  _logger->flush();

  Setting setting(dict, keyRecom, plp);
  WebServer server(10, 10, "127.0.0.1", 8888, logger, config, setting, cache);
  server.Start();
}
