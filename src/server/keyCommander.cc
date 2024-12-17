/**
 * @file    :keyCommander.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-19
 */

#include "cache/redis.h"
#include "key/dictionary.h"
#include "reactor/key_command_server.h"

using namespace hh;

int main() {
  hh::Logger logger("/home/hh/searchEngine/log/server_test.log");
  Configuration config("/home/hh/searchEngine/conf/configure.json", logger);
  RedisCache cache(logger, config);
  Dictionary dict(logger, config);
  KeyRecommander keyRecom(dict);
  keyRecom.Query("你好");
  keyRecom.Print();
  PageLib pl(logger, config);
  PageLibPreprocessor plp(pl, logger, config);
  pl.Load();
  plp.CreatePageLibList();
  plp.RmRedundantPages();
  plp.Cut();
  plp.BuildInvertIndexTable();
  plp.LoadIndexPageList();

  auto _logger = logger.getLogger(); // 强制刷新
  _logger->flush();

  Setting setting(dict, keyRecom, plp, cache);
  WebServer server(10, 10, "127.0.0.1", 8888, logger, config, setting);
  server.Start();
}
