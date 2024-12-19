/**
 * @file    :lru_cache.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-123
 */

#ifndef __LRU_CACHE_H__
#define __LRU_CACHE_H__

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

#include <list>
#include <string>
#include <unordered_map>
#include <utility>

namespace hh {
using nlohmann::json;
using std::list;
using std::pair;
using std::string;
using std::unordered_map;

class LRUCache {
public:
  explicit LRUCache(int capacity = 100);
  LRUCache(const LRUCache &cache);

  bool Get(string key, json &value);
  void Put(string key, json &value);

  void WriteToFile(string filename);
  void WriteToFile(string filename, string key, json value);

  void ReadFromFile(string filename);

  void Update(const LRUCache &);

  void show();

  list<pair<string, json>> &GetPendingUpdateList();
  list<pair<string, json>> &GetResultList();

private:
  unordered_map<string, list<pair<string, json>>::iterator>
      _hash_map;                                 // 使用hashTable进行查找
  list<pair<string, json>> _result_list;         // 保存键值对
  list<pair<string, json>> _pending_update_list; // 等待更新的节点信息
  int _capacity;
  bool _isUpdating;
};

} // namespace hh

#endif // LRU_CACHE_H