/**
 * @file    :key_recommander.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-18
 */

#ifndef __KEY_RECOMMANDER_H__
#define __KEY_RECOMMANDER_H__

#include "key/candidate_result.h"
#include "key/dict_producer.h"
#include "key/dictionary.h"

#include <queue>
#include <string>

namespace hh {
using std::priority_queue;

struct Compare {
  bool operator()(CandidateResult &lhs, CandidateResult &rhs) {
    bool flag;
    if (lhs._dist == rhs._dist) {
      flag = (lhs._freq < rhs._freq); // 词频大的优先
    } else if (lhs._dist < rhs._dist) {
      flag = false;
    } else {
      flag = true;
    }
    return flag;
  }
};
class KeyRecommander {
public:
  explicit KeyRecommander(Dictionary &dic);
  void Query(const std::string &word);
  void Print();
  string GetString();

private:
  int MinEditDist(const std::string &rhs);

private:
  priority_queue<CandidateResult, vector<CandidateResult>, Compare> _pq;
  Dictionary &_dic;
  string _sought;
  vector<string> _result;
  int _word_num;
};
} // namespace hh

#endif // KEY_REMOCCANDER_H