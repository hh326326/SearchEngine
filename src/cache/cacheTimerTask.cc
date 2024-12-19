/**
 * @file    :cacheTimerTask.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#include "cache/cacheTimerTask.h"
#include "cache/cache_manager.h"

namespace hh {
// 缓存更新
void CacheTimerTask::process() {
  CacheManager *cache_manager = CacheManager::GetCacheManager();
  cache_manager->PeriodicUpdateCaches();
}
} // namespace hh