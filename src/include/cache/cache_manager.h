/**
 * @file    :cache_manager.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-16
 */

#ifndef __CACHE_MANAGER_H__
#define __CACHE_MANAGER_H__

#include "cache/lru_cache.h"
#include <string>
#include <vector>

namespace hh {
using std::string;
using std::vector;

class CacheManager {
public:
  void Init(const string &file);
  LruCache &GetCache(int);
  void UpdateCache();

private:
  vector<LruCache> _caches;
};

} // namespace hh

#endif // CACHE_MANAGER_H