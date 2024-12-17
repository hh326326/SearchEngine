/**
 * @file    :redis.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-16
 */

#ifndef __REDIS_H__
#define __REDIS_H__

#include "common/configuration.h"
#include <cstdint>
#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>

namespace hh {
using std::string;
using sw::redis::Redis;

class RedisCache {
public:
  RedisCache(Logger &log, IConfiguration &conf);
  string DoQuery(const string &key, bool flag);
  void AddData(const string &key, const string &value, bool flag);

private:
  Logger &_logger;
  IConfiguration &_conf;
  Redis _key_redis;
  Redis _web_redis;
  int64_t _ttl;
};

} // namespace hh

#endif // REDIS_H