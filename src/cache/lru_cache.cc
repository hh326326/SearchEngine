/**
 * @file    :lru_cache.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-14
 */

#include "cache/lru_cache.h"

#include <iostream>
#include <string>
#include <utility>

namespace hh {
LruCache::LruCache(int cache_size) : _cache_size(cache_size) {}

void LruCache::ReadFromFile(const string &file) {
  // 匹配失败的?
}

void LruCache::AddRecord(const string &key, const string &result) {
  auto it = _hash_map.find(key);
  if (it != _hash_map.end()) {
    // 在缓存中
    // list移动元素, hashmap修改iterator

    // 1. 移动并更新hash map
    MoveToFront(key);
    // 2. 更新value
    _result_list.begin()->second = result;
  } else {
    // 不在缓存中
    // if (full)
    // delete
    // add to list
    // add to pendinglist

    if (_result_list.size() == _cache_size) {
      // delete rear
      DeleteRear();
    }
    // add
    AddFront(key, result);

    // pending list
    AddToPending(key, result);
  }
}

void LruCache::Update(const LruCache &) {}

void LruCache::WriteToFile(const string &file) {}

void LruCache::LruTest() {
  for (auto &elem : _result_list) {
    std::cout << elem.first << " " << elem.second << '\n';
  }
  std::cout << '\n';
  for (auto &elem : _hash_map) {
    std::cout << elem.first << " " << elem.second->first << '\n';
  }
  std::cout << "\n\n\n";
}

// add front with hashmap update
void LruCache::AddFront(const string &key, const string &value) {
  // add item to list
  _result_list.emplace_front(std::make_pair(key, value));
  // update hash map
  _hash_map.emplace(key, _result_list.begin());
}

void LruCache::DeleteRear() {
  // get key of rear item
  string key = _result_list.back().first;
  // delete from list
  _result_list.pop_back();
  // delete from hash with key
  _hash_map.erase(key);
}

void LruCache::MoveToFront(const string &key) {
  // get iterator
  auto it = _hash_map[key];
  // save content
  pair<string, string> tmp = *it;
  // delete from list
  _result_list.erase(it);
  // add to front
  _result_list.emplace_front(tmp);
  // update hash
  _hash_map[key] = _result_list.begin();
}

void LruCache::AddToPending(const string &key, const string &value) {
  if (_pending_update_list.size() == _cache_size) {
    _pending_update_list.pop_back();
  }
  _pending_update_list.emplace_front(std::make_pair(key, value));
}

} // namespace hh
