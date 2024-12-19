/**
 * @file    :cache_manager.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-27
 */

#ifndef __CACHE_MANAGER_H__
#define __CACHE_MANAGER_H__

#include "cache/lru_cache.h"
#include <cstddef>
#include <pthread.h>
#include <string>
#include <vector>
#include "common/logger.h"
#include "common/configuration.h"

namespace hh {
using std::string;
using std::vector;

class CacheManager {
public:
  static CacheManager *GetCacheManager();
  static void Init();
  static void Destory();
  void InitCache(size_t threadNum, string wordCacheFile, string webCacheFile);

  LRUCache &GetKeyWordCache(int); // 从磁盘文件中读取缓存信息
  LRUCache &GetWebPageCache(int); // 从磁盘文件中读取缓存信息

  void PeriodicUpdateCaches(); // 定时更新所有缓存
private:
  CacheManager();
  ~CacheManager(){};
  static CacheManager *_cache_manager;
  static pthread_once_t _once;
  vector<LRUCache> _key_word_caches;
  vector<LRUCache> _web_page_caches;

};

} // namespace hh

#endif // CACHE_MANAGER_H