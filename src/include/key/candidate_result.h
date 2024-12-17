/**
 * @file    :candidate_result.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-15
 */

#ifndef __CANDIDATE_RESULT_H__
#define __CANDIDATE_RESULT_H__

#include <string>

namespace hh {

using std::string;

class CandidateResult {
public:
  CandidateResult() = default;

  ~CandidateResult() = default;

  string _word;
  int _freq;
  int _dist;
};
} // namespace hh

#endif // candidate_result_H