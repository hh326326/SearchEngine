/**
 * @file    :calculate.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-14
 */

#ifndef __CALCULATE_H__
#define __CALCULATE_H__

#include <cmath>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <set>

namespace hh {
using std::string;
using std::unordered_map;
using TF_IDF_Weight = double;
using DocId = uint64_t;

//
// 计算一篇文档中所有单词TF_IDF的值
// terms_frequency_map
// invert_index_table 倒排索引表
// total_docs_num 总文档数量

unordered_map<string, TF_IDF_Weight>
CalculateValue(const unordered_map<string, uint64_t> &terms_frequency_map,
              unordered_map<string, std::set<std::pair<DocId, TF_IDF_Weight>>>& invert_index_table,
               uint64_t total_docs_num);

} // namespace hh

#endif // CALCULATE_H