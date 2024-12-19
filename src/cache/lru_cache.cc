/**
 * @file    :lru_cache.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-14
 */

#include "cache/lru_cache.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace hh {

LRUCache::LRUCache(int capacity) : _capacity(capacity), _isUpdating(false) {}

LRUCache::LRUCache(const LRUCache &cache) : _capacity(cache._capacity) {
  Update(cache);
}

// 查找_resultList中有无key值，若有，传出value
bool LRUCache::Get(string key, json &value) {
  auto it = _hash_map.find(key);
  if (it == _hash_map.end()) {
    return false;
  }
  _result_list.splice(_result_list.begin(), _result_list, it->second);
  _pending_update_list.remove(*(it->second));
  _pending_update_list.push_front(
      std::move(std::make_pair(it->second->first, it->second->second)));
  value = it->second->second;

  return true;
}

// 将 key-value放入缓存中
void LRUCache::Put(string key, json &value) {
  auto it = _hash_map.find(key);
  if (it == _hash_map.end()) {
    if (_hash_map.size() >= _capacity) {
      _hash_map.erase(_result_list.back().first);
      _result_list.pop_back();
    }
    _result_list.push_front(std::move(std::make_pair(key, value)));
    _hash_map[key] = _result_list.begin();

    _pending_update_list.push_front(std::move(std::make_pair(key, value)));
  } else {
    _result_list.splice(_result_list.begin(), _result_list, it->second);
  }
}

void LRUCache::WriteToFile(string filename) {
  std::ofstream ofs(filename);
  if (!ofs.good()) {
    std::cerr << "error: " << filename << '\n';
  }
  for (auto &it : GetResultList()) {
    ofs << it.first << " " << it.second << '\n';
  }
  ofs.close();
}

void LRUCache::WriteToFile(string filename, string key, json value) {
  std::ofstream ofs(filename, std::ios::app);
  if (!ofs.good()) {
    std::cerr << "error: " << filename << '\n';
  }
  ofs << key << " " << value << '\n';
  ofs.close();
}

void LRUCache::ReadFromFile(string filename) {
  std::ifstream ifs(filename);
  if (!ifs.good()) {
    std::cerr << "error: " << filename << '\n';
  }
  string line;
  while (std::getline(ifs, line)) {
    std::istringstream iss(line);
    string key;
    json value;
    iss >> key;
    iss >> value;
    Put(key, value);
  }
  ifs.close();
}

void LRUCache::Update(const LRUCache &cache) {
  _isUpdating = true;
  int count = 0;
  auto it = cache._pending_update_list.rbegin();
  for (int distance = _pending_update_list.size() - _result_list.size();
       distance > 0; --distance) {
    ++it;
  }
  for (; (it != cache._pending_update_list.rend()); ++it) {
    json j = it->second;
    Put(it->first, j);
    ++count;
    if (count >= _capacity) {
      _isUpdating = false;
      return;
    }
  }
  _isUpdating = false;
}

void LRUCache::show() {
  for (auto it = _result_list.begin(); it != _result_list.end(); ++it) {
    std::cout << "key: " << it->first << " ";
    std::cout << "value: ";
    for (auto i : it->second) {
      std::cout << i << " ";
    }
    std::cout << '\n';
  }
}

list<pair<string, json>> &LRUCache::GetPendingUpdateList() {
  return _pending_update_list;
}

list<pair<string, json>> &LRUCache::GetResultList() { return _result_list; }

} // namespace hh
