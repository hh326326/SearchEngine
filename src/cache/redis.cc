/**
 * @file    :redis.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-14
 */

#include "cache/redis.h"
#include "common/configuration.h"
#include "common/logger.h"
#include <cstdlib>
#include <iostream>
#include <sw/redis++/redis++.h>

namespace hh {
using sw::redis::OptionalString;

RedisCache::RedisCache(Logger &log, IConfiguration &conf)
    : _logger(log), _conf(conf),
      _key_redis(_conf.GetConfig()["redis_key.init"]),
      _web_redis(_conf.GetConfig()["redis_web.init"]),
      _ttl(std::atoi(_conf.GetConfig()["redis.ttl"].c_str())) {
  LOG_INFO("Redis cache connected");
}

string RedisCache::DoQuery(const string &key, bool flag) {
  Redis &redis = flag ? _key_redis : _web_redis;
  OptionalString value = redis.get(key);
  if (value) {
    // 存在
    LOG_INFO("Query key:\"{}\" Cache hit.", key);
    redis.expire(key, _ttl);
    return value.value();
  }
  // 不存在, 返回空字符串
  std::cout << "hello" << '\n';
  LOG_INFO("Query key:\"{}\" Cache missed.", key);
  return "";
}

void RedisCache::AddData(const string &key, const string &value, bool flag) {
  Redis &redis = flag ? _key_redis : _web_redis;
  redis.setex(key, _ttl, value);
  LOG_INFO("New data added to cache. key:\"{}\"", key);
}

} // namespace hh
