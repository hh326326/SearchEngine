/**
 * @file    :web_page_query.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-16
 */

#include "web/web_page_query.h"
#include "key/dictionary.h"
#include "web/calculate.h"
#include <cmath>
#include <complex>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hh {
WebPageQuery::WebPageQuery(const PageLibPreprocessor &pre, Logger &logger,
                           IConfiguration &config)
    : _logger(logger), _config(config), _pre(pre),
      _spilt_tool(_logger, _config) {}

// 加载库文件
// void WebPageQuery::LoadLibrary();

// 返回查询结果
string WebPageQuery::DoQuery(const string &sought) {
  unordered_map<string, uint64_t> query_words = Cut(sought);

  unordered_map<string, TF_IDF_Weight> base_vec =
      CalculateQueryWeights(query_words);
  auto relative_pages_vectors = CalculateRelativePagesVectors(query_words);
  if (relative_pages_vectors.empty()) {
    return NoAnswer();
  }
  vector<DocId> ordered_pages = GetPagesOrder(base_vec, relative_pages_vectors);
  return CreateJson(ordered_pages);
}
// 通过TF-IDF算法算出每个关键词的权重系数
unordered_map<string, TF_IDF_Weight> WebPageQuery::CalculateQueryWeights(
    const unordered_map<string, uint64_t> &query_words) {
  return CalculateValue(query_words, _pre._document_frequencies,
                        _pre._page_lib_list.size());
}

// 通过倒排索引表查询包含所有关键词的网页:只要其中有一个查询词不在索引表中，就认为没有找到相关网页
vector<DocId> WebPageQuery::GetRelativePages(
    const unordered_map<string, uint64_t> &query_words) {
  vector<DocId> relative_pages;
  unordered_map<DocId, uint64_t> document_frequencies;
  // 获取查询词个数
  uint64_t query_words_num = query_words.size();
  document_frequencies.reserve(query_words_num);
  for (const auto &pair : query_words) {
    auto it = _pre._invert_index_table.find(pair.first);
    // 只要有一个查询词不在索引表，就认为没有找到相关网页
    if (it == _pre._invert_index_table.end()) {
      return vector<DocId>();
    }
    // 查找包含查询词的所有网页
    for (const auto &doc_id_set : it->second) {
      // 记录相关网页是否在结构集合中
      auto it_doc_freq = document_frequencies.find(doc_id_set.first);
      if (it_doc_freq == document_frequencies.end()) {
        document_frequencies.insert(std::make_pair(doc_id_set.first, 1));
      } else {
        // 标记数加1
        it_doc_freq->second += 1;
      }
    }
  }
  // 遍历DF, fre == query_words_num, 就是相关网页
  for (const auto &pair : document_frequencies) {
    if (pair.second == query_words_num) {
      relative_pages.push_back(pair.first);
    }
  }
  return relative_pages;
}

// 算出相关网页的查询词向量集合
DocsVector WebPageQuery::CalculateRelativePagesVectors(
    const unordered_map<string, uint64_t> &query_words) {
  auto relative_pages = GetRelativePages(query_words);
  DocsVector relative_pages_vectors;
  relative_pages_vectors.reserve(relative_pages.size());

  for (const auto &doc_id : relative_pages) {
    // auto page = _pre._page_lib_list[doc_id]; // error
    // _page_lib_list并没有按照DocId建立索引
    auto page = _pre._index_page_lib_list.find(doc_id)->second;
    auto page_all_vectors =
        CalculateValue(page._term_frequencies, _pre._document_frequencies,
                       _pre._page_lib_list.size());
    // LOG_DEBUG("page_all_vectors:");
    // for (const auto& tf_idf_vector : page_all_vectors) {
    //   LOG_DEBUG("{} {}", tf_idf_vector.first, tf_idf_vector.second);
    // }

    // 去除page_all_vectors中没有在query_words中的词
    unordered_map<string, TF_IDF_Weight> page_query_vectors;
    page_query_vectors.reserve(query_words.size());
    for (const auto &pair : query_words) {
      auto it = page_all_vectors.find(pair.first);
      if (it == page_all_vectors.end()) {
        continue;
      }
      page_query_vectors.insert(std::make_pair(pair.first, it->second));
    }
    relative_pages_vectors.emplace_back(
        std::make_pair(doc_id, page_query_vectors));
  }
  return relative_pages_vectors;
}
// 计算余弦相似度
// 公式: Cosine = (X * Y) / (|X| * |Y|)
Cosine WebPageQuery::CalculateCosSimilarity(
    const unordered_map<string, TF_IDF_Weight> &x,
    const unordered_map<string, TF_IDF_Weight> &y) {
  // X * Y = (x1 * y1 + x2 * y2 + ...)
  double sum = 0.0;
  for (const auto &pair : x) {
    auto it = y.find(pair.first);
    if (it == y.end()) {
      throw std::runtime_error("CalculateCosSimilarity error");
    }
    sum += pair.second * it->second;
  }
  // |X|
  double x_norm = 0.0;
  for (const auto &pair : x) {
    x_norm += pair.second * pair.second;
  }
  x_norm = std::sqrt(x_norm);
  // |Y|
  double y_norm = 0.0;
  for (const auto &pair : y) {
    y_norm += pair.second * pair.second;
  }
  y_norm = std::sqrt(y_norm);

  // Cosine = (X * Y) / (|X| * |Y|)
  return sum / (x_norm * y_norm);
}

// 对查找到的网页进行排序，排序算法采用余弦相似度算法
vector<DocId> WebPageQuery::GetPagesOrder(
    const unordered_map<string, TF_IDF_Weight> &base_vec,
    const DocsVector &relative_pages_vectors) {
  map<double, DocId, std::greater<>> ordered_pages;
  for (const auto &pair : relative_pages_vectors) {
    auto cos_similarity = CalculateCosSimilarity(base_vec, pair.second);
    ordered_pages.insert(std::make_pair(cos_similarity, pair.first));
  }
  vector<DocId> result;
  result.reserve(ordered_pages.size());
  for (const auto &pair : ordered_pages) {
    result.push_back(pair.second);
  }
  return result;
}

// 封装相关网址的标题和摘要信息为一个Json字符串
string WebPageQuery::CreateJson(vector<DocId> &doc_ids) {
  string result;
  for (const auto &doc_id : doc_ids) {
    result.append("------------------------------\n");

    result.append("<title> ");
    result.append(
        _pre._index_page_lib_list.find(doc_id)->second._rss_item.title);
    result.append("\n");
    result.append("<abstract> ");
    result.append(_pre._index_page_lib_list.find(doc_id)->second._doc_abstract);
    result.append("\n");
    result.append("<link> ");
    result.append(
        _pre._index_page_lib_list.find(doc_id)->second._rss_item.link);
    result.append("\n");
    result.append("\n");
  }
  return result;
}

string WebPageQuery::NoAnswer() { return "No answer\n"; }

unordered_map<string, uint64_t> WebPageQuery::Cut(const string &sought) {
  unordered_map<string, uint64_t> result;
  _spilt_tool.Cut(sought, result, true);
  return result;
}
} // namespace hh