/**
 * @file    :web_page_query.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-11
 */

#ifndef __WEB_PAGE_QUERY_H__
#define __WEB_PAGE_QUERY_H__

#include "common/configuration.h"
#include "common/split_tool.h"
#include "key/dictionary.h"
#include "web/calculate.h"
#include "web/page_lib_preprocessor.h"
#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hh {
using Cosine = double;
using DocsVector = vector<pair<DocId, unordered_map<string, TF_IDF_Weight>>>;

struct SendInfo {
  string title;
  string abstracts;
  string link;
  string author;
};

class WebPageQuery {
public:
  WebPageQuery(
               IConfiguration &config);
  // 加载库文件
  void LoadLibrary();
  // 返回查询结果
  vector<string> DoQuery(const string &);
  // 通过TF-IDF算法算出每个关键词的权重系数
  unordered_map<string, TF_IDF_Weight>
  CalculateQueryWeights(const unordered_map<string, uint64_t> &query_words);
  // 通过倒排索引表查询包含所有关键词的网页:只要其中有一个查询词不在索引表中，就认为没有找到相关网页
  vector<DocId>
  GetRelativePages(const unordered_map<string, uint64_t> &query_words);
  // 算出相关网页的查询词向量集合
  DocsVector CalculateRelativePagesVectors(
      const unordered_map<string, uint64_t> &query_words);
  // 计算余弦相似度
  // 公式: Cosine = (X * Y) / (|X| * |Y|)
  static Cosine
  CalculateCosSimilarity(const unordered_map<string, TF_IDF_Weight> &,
                         const unordered_map<string, TF_IDF_Weight> &);
  // 对查找到的网页进行排序，排序算法采用余弦相似度算法
  static vector<DocId>
  GetPagesOrder(const unordered_map<string, TF_IDF_Weight> &,
                const DocsVector &);
  // 封装相关网址的标题和摘要信息为一个Json字符串
  vector<string> CreateJson(vector<DocId> &doc_ids);
  vector<string> NoAnswer();
  unordered_map<string, uint64_t> Cut(const string &sought);

public:
  IConfiguration &_config;

  // 网页偏移库
  unordered_map<DocId, pair<uint64_t, uint64_t>> _offset_lib;
  // 倒排索引库
  unordered_map<string, set<pair<DocId, TF_IDF_Weight>> > _invert_index_table;
  unordered_map<DocId, SendInfo> _index_page_lib_list;
private:
  
  SplitTool _spilt_tool;
};
} // namespace hh

#endif // WEB_PAGE_QUERY_H