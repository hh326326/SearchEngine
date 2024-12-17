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

namespace hh {
using std::string;
using std::unordered_map;
using TF_IDF_Weight = double;

//
// 计算一篇文档中所有单词TF_IDF的值
// terms_frequency_map
// document_frequency 包含该单词的文档数量
// total_docs_num 总文档数量

unordered_map<string, TF_IDF_Weight>
CalculateValue(const unordered_map<string, uint64_t> &terms_frequency_map,
               unordered_map<string, uint64_t> document_frequency,
               uint64_t total_docs_num);

} // namespace hh

#endif // CALCULATE_H