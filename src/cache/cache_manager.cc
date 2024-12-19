/**
 * @file    :cache_manager.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-27
 */

#include "cache/cache_manager.h"
#include "cache/lru_cache.h"
#include "common/config_one.h"
#include "common/configuration.h"
#include <cstddef>
#include <cstdlib>
#include <pthread.h>
#include <string>

namespace hh {
CacheManager *CacheManager::_cache_manager = nullptr;
pthread_once_t CacheManager::_once = PTHREAD_ONCE_INIT;

CacheManager *CacheManager::GetCacheManager() {
  pthread_once(&_once, Init);
  return _cache_manager;
}

void CacheManager::Init() {
  _cache_manager = new CacheManager();
  atexit(Destory);
}

void CacheManager::Destory() {
  if (_cache_manager) {
    LRUCache &lru1 = _cache_manager->GetKeyWordCache(0);
    LRUCache &lru2 = _cache_manager->GetWebPageCache(0);

    delete _cache_manager;
    _cache_manager = nullptr;
  }
}

void CacheManager::InitCache(size_t threadNum, string wordCacheFile,
                             string webCacheFile) {
  _key_word_caches.resize(threadNum);
  _web_page_caches.resize(threadNum);

  for (auto it = _key_word_caches.begin(); it != _key_word_caches.end(); ++it) {
    it->ReadFromFile(wordCacheFile);
  }
  for (auto it = _web_page_caches.begin(); it != _web_page_caches.end(); ++it) {
    it->ReadFromFile(webCacheFile);
  }
  // test
  //_keyWordCaches.begin()->show();
  // cout << _keyWordCaches.size() << endl;

#if 0
    for(auto iter : _keyWordCaches)
    {
        iter.readFromFile(wordCacheFile);
    }
    for(auto iter : _webPageCaches)
    {
        iter.readFromFile(webCacheFile);
    }
#endif
}

// 从磁盘文件中读取缓存信息
LRUCache &CacheManager::GetKeyWordCache(int idx) {
  return _key_word_caches[idx];
}

// 从磁盘文件中读取缓存信息
LRUCache &CacheManager::GetWebPageCache(int idx) {
  return _web_page_caches[idx];
}

// 定时更新所有缓存
void CacheManager::PeriodicUpdateCaches() {
  map<string, string> config_map = ConfigOne::getConfigInstance()->GetConfig();
  //   map<string, string>& config_map =
  LRUCache &mainKeyCache = _key_word_caches[0];
  for (int idx = 0; idx != _key_word_caches.size(); ++idx) {
    auto &pendingUpdatelist = _key_word_caches[idx].GetPendingUpdateList();
    for (auto &iter : pendingUpdatelist) {
      mainKeyCache.Put(iter.first, iter.second);
    }
    pendingUpdatelist.clear();
  }
  // mainKeyCache.WriteToFile(conf.GetConfig()["key_word_cache"])
  mainKeyCache.WriteToFile(config_map["key_word_cache"]);
  // mainKeyCache.WriteToFile("/home/hh/searchEngine/data/cache/key_word_cache.dat");

  for (int idx = 1; idx != _key_word_caches.size(); ++idx) {
    _key_word_caches[idx].Update(mainKeyCache);
  }

  LRUCache &mainWebCache = _web_page_caches[0];
  for (int idx = 0; idx != _web_page_caches.size(); ++idx) {
    auto &pendingUpdatelist = _web_page_caches[idx].GetPendingUpdateList();
    for (auto &iter : pendingUpdatelist) {
      mainWebCache.Put(iter.first, iter.second);
    }
    pendingUpdatelist.clear();
  }
  //   mainWebCache.WriteToFile(conf.GetConfig()["web_page_cache"]);
  mainWebCache.WriteToFile(config_map["web_page_cache"]);
  // mainKeyCache.WriteToFile("/home/hh/searchEngine/data/cache/web_page_cache.dat");

  for (int idx = 1; idx != _web_page_caches.size(); ++idx) {
    _web_page_caches[idx].Update(mainWebCache);
  }
#if 0
    for(auto iter : _keyWordCaches)
    {   
        iter.update(_keyWordCaches[0]);
    }

    for(auto iter : _webPageCaches)
    {   
        iter.update(_webPageCaches[0]);
    }
#endif
}

CacheManager::CacheManager() {
  map<string, string> config_map = ConfigOne::getConfigInstance()->GetConfig();
  size_t threadNum = std::atoi(config_map["threadNum"].c_str());
  InitCache(threadNum, config_map["key_word_cache"],
            config_map["web_page_cache"]);
}

} // namespace hh