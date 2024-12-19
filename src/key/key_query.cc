/**
 * @file    :key_query.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-15
 */

#include "key/key_query.h"
#include "key/candidate_result.h"
#include "key/dictionary.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace hh {

using std::pair;
using std::string;
using std::vector;

KeyQuery::KeyQuery(Dictionary &dic)
    : _dic(dic), _sought(), _word_num(200) {}

void KeyQuery::Query(const std::string &word) {
  _sought = std::move(word);
  // 行号，频数
  // std::cout << "sought = " << _sought << '\n';
  unordered_map<int, int> word_map(_dic.DoQuery(_sought));
  // std::cout << "word_map.size() = " << word_map.size() << '\n';

  CandidateResult result;
  if (hh::Dictionary::CharLen(_sought) == 3) {
    // 加载中文词典
    // std::cout << "输入是中文" << '\n';
    vector<pair<string, int>> &dict = _dic.GetZhDict();
    for (const auto &elem : word_map) {

      result._word = dict[elem.first].first;
      result._freq = dict[elem.second].second;
      result._dist = MinEditDist(result._word);
      // std::cout << "word = " << result._word << '\n';
      _pq.emplace(result);
    }
  } else {
    // 加载英文词典
    // std::cout << "输入是英文" << '\n';
    vector<pair<string, int>> &dict = _dic.GetEnDict();
    for (const auto &elem : word_map) {
      result._word = dict[elem.first].first;
      result._freq = dict[elem.second].second;
      result._dist = MinEditDist(result._word);
      // std::cout << "word = " << result._word << '\n';
      _pq.emplace(result);
    }
  }
}

// test
void KeyQuery::Print() {
  int cnt = 1;
  for (size_t i = 0; i < _pq.size(); ++i) {
    std::cout << "word: " << _pq.top()._word << " dist: " << _pq.top()._dist
              << " freq: " << _pq.top()._freq << '\n';
    _pq.pop();
    if (cnt == 5) {
      break;
    }
    ++cnt;
  }
  for (size_t i = 0; i < _pq.size(); ++i) {
    _pq.pop();
  }
}

vector<string> KeyQuery::GetString() {
  _result.clear();
  // 保存优先队列前五
  for (int i = 0; i < _pq.size(); ++i) {
    _result.push_back(_pq.top()._word);
    _pq.pop();
    if (i == 4) {
      break;
    }
  }
  for (int i = 0; i < _pq.size(); ++i) {
    _pq.pop();
  }
  vector<string> result;
  for (const auto &elem : _result) {
    result.push_back(elem);
  }
  return result;
}

int KeyQuery::MinEditDist(const std::string &rhs) {
  const string &lhs = _sought;
  int char_len = hh::Dictionary::CharLen(rhs);
  int lhs_len = static_cast<int>(lhs.length()) / char_len;
  int rhs_len = static_cast<int>(rhs.length()) / char_len;

  if (0 == lhs_len * rhs_len) {
    return lhs_len + rhs_len;
  }

  vector<vector<int>> distance(lhs_len + 1, vector<int>(rhs_len + 1));

  for (int i = 0; i < lhs_len + 1; ++i) {
    distance[i][0] = i;
  }
  for (int i = 0; i < rhs_len + 1; ++i) {
    distance[0][i] = i;
  }

  for (int i = 1; i < lhs_len + 1; ++i) {
    for (int j = 1; j < rhs_len + 1; ++j) {
      int left = distance[i - 1][j] + 1;
      int down = distance[i][j - 1] + 1;
      int left_down = distance[i - 1][j - 1];
      if (string(lhs, (i - 1) * char_len, char_len) !=
          string(rhs, (j - 1) * char_len, char_len)) {
        ++left_down;
      }
      distance[i][j] = std::min(left, std::min(down, left_down));
    }
  }
  return distance[lhs_len][rhs_len];
}

} // namespace hh
