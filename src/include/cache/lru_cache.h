/**
 * @file    :lru_cache.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-16
 */

#ifndef __LRU_CACHE_H__
#define __LRU_CACHE_H__

#include <list>
#include <string>
#include <unordered_map>
#include <utility>

namespace hh {
using std::list;
using std::pair;
using std::string;
using std::unordered_map;

class LruCache {
public:
  explicit LruCache(int cache_size = 100);
  void ReadFromFile(const string &file);
  void AddRecord(const string &key, const string &result);
  void Update(const LruCache &);
  void WriteToFile(const string &file);

  void LruTest();

private:
  void AddFront(const string &key, const string &value);
  void DeleteRear();
  void MoveToFront(const string &key);
  void AddToPending(const string &key, const string &value);

private:
  list<pair<string, string>> _result_list;
  unordered_map<string, list<pair<string, string>>::iterator> _hash_map;
  unordered_map<int, list<pair<string, string>>::iterator> _freq_map;
  list<pair<string, string>> _pending_update_list;
  int _cache_size;
  int _min_freq;
};

} // namespace hh

#endif // LRU_CACHE_H