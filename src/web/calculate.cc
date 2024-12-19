/**
 * @file    :calculate.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-15
 */

#include "web/calculate.h"
#include <cmath>
#include <cstdint>
#include <unordered_map>

namespace hh {

unordered_map<string, TF_IDF_Weight>
CalculateValue(const unordered_map<string, uint64_t> &terms_frequency_map,
               unordered_map<string, std::set<std::pair<DocId, TF_IDF_Weight>>>& invert_index_table,
               uint64_t total_docs_num) {
  unordered_map<string, TF_IDF_Weight> doc_weights;
  doc_weights.reserve(terms_frequency_map.size());

  for (const auto &pair : terms_frequency_map) {
    uint64_t term_frequency = pair.second;
    uint64_t document_frequency = invert_index_table[pair.first].size();
    // term_frequency 单词在文档中出现的次数
    // document_frequency 单词在整个文档库中出现的次数
    double inverse_document_frequency = 0; //< IDF = log2(N / (DF + 1))
    TF_IDF_Weight weight = 0;              //< w = TF * IDF

    // IDF = log2(N / (DF + 1))
    inverse_document_frequency =
        log2(static_cast<double>(total_docs_num) /
             static_cast<double>(document_frequency + 1));
    weight = static_cast<double>(term_frequency) * inverse_document_frequency;
    doc_weights.emplace(pair.first, weight);
  }
  // 对w归一化处理, 得出w`
  double weight_sum = 0;
  for (auto &pair : doc_weights) {
    weight_sum += pair.second * pair.second;
  }
  // sqrt(w1^2 + w2^2 + ... + wn^2)
  double after_sqrt = std::sqrt(weight_sum);
  for (auto &pair : doc_weights) {
    //  w` = w / sqrt(w1^2 + w2^2 + ... + wn^2)
    pair.second = pair.second / after_sqrt;
  }
  return doc_weights;
}

} // namespace hh