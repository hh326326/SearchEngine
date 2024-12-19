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
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hh {
WebPageQuery::WebPageQuery( IConfiguration &config)
    :  _config(config), _spilt_tool(_config) {
  LoadLibrary();
}

// 加载网页偏移库和倒排索引库
void WebPageQuery::LoadLibrary() {
  // 读取网页偏移库
  std::ifstream ifs_offset_lib(
      "/home/hh/searchEngine/data/database/new_offset_lib.dat");
  if (!ifs_offset_lib) {
    std::cerr << "Errod: open new_offset.dat fail." << '\n';
  }
  string line;
  while (std::getline(ifs_offset_lib, line)) {
    std::istringstream iss(line);
    int docid, docbegin, doclen;
    iss >> docid >> docbegin >> doclen;
    _offset_lib[docid] = std::make_pair(docbegin, doclen);
  }
  ifs_offset_lib.close();

  // 读取倒排索引库
  std::ifstream ifs_invert_index_table(
      "/home/hh/searchEngine/data/database/invertindex.dat");
  if (!ifs_invert_index_table) {
    std::cerr << "Errod: open invertindex.dat fail." << '\n';
  }
  while (getline(ifs_invert_index_table, line)) {
    std::istringstream iss(line);
    string word;
    iss >> word;
    set<pair<DocId, TF_IDF_Weight>> index;
    int docid;
    TF_IDF_Weight weight;
    while (iss >> docid >> weight) {
      index.insert({docid, weight});
    }
    _invert_index_table[word] = index;
  }
  ifs_invert_index_table.close();

  // 加载 _index_page_lib_list
  std::ifstream ifs_page_lib_list(_config.GetConfig()["ripe_page_send"]);
  if (!ifs_page_lib_list) {
    std::cerr << "Errod: open ripe_page_send.dat fail." << '\n';
  }
  while (getline(ifs_page_lib_list, line)) {
    // 解析每一条记录
    std::stringstream ss(line);
    DocId docId;
    string title, abstracts, link, author;

    // 读取 docId
    ss >> docId;
    ss.ignore(); // 忽略一个空格或换行符

    // 读取标题 (直到找到换行符为止)
    getline(ifs_page_lib_list, title);

    // 读取摘要（假设摘要一直在标题下方，直到下一行开始链接）
    getline(ifs_page_lib_list, abstracts);

    // 读取链接（最后一行是链接）
    getline(ifs_page_lib_list, link);

    // 读取作者（假设作者信息在链接之后的一行）
    getline(ifs_page_lib_list, author);

    // 将数据填充到 SendInfo 结构体中
    SendInfo info = {title, abstracts, link, author};

    // 将数据存储到 _index_page_lib_list 中
    _index_page_lib_list[docId] = info;
  }
  ifs_page_lib_list.close();
}

// 返回查询结果
vector<string> WebPageQuery::DoQuery(const string &sought) {
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
  return CalculateValue(query_words, _invert_index_table,
                        _index_page_lib_list.size());
}

// 通过倒排索引表查询包含所有关键词的网页:只要其中有一个查询词不在索引表中，就认为没有找到相关网页
vector<DocId> WebPageQuery::GetRelativePages(
    const unordered_map<string, uint64_t> &query_words) {
  vector<DocId> relative_pages;
  unordered_map<DocId, uint64_t> document_frequencies;
  // 获取查询词个数
  uint64_t query_words_num = query_words.size();
  document_frequencies.reserve(query_words_num);
  int un_contain_words_num = 0;
  for (const auto &pair : query_words) {
    auto it = _invert_index_table.find(pair.first);
    // 只要有超过一半查询词不在索引表，就认为没有找到相关网页
    if (it == _invert_index_table.end()) {
      ++un_contain_words_num;
      if (un_contain_words_num > query_words.size() / 2) {
           return vector<DocId>();
      }
     
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

  unordered_map<string, TF_IDF_Weight> cols;
  for (const auto &doc_id : relative_pages) {
    cols.clear();
    for (auto &[key, freq] : query_words) {
      set<pair<DocId, TF_IDF_Weight>> st = _invert_index_table[key];
      for (auto &[id, weight] : st) {
        if (id == doc_id) {
          cols[key] = weight;
        }
      }
    }
    relative_pages_vectors.emplace_back(std::make_pair(doc_id, cols));
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
vector<string> WebPageQuery::CreateJson(vector<DocId> &doc_ids) {
  vector<string> result;
  for (const auto &doc_id : doc_ids) {
    string doc;
    doc.append("------------------------------\n");

    doc.append("<title> ");
    doc.append(_index_page_lib_list.find(doc_id)->second.title);
    doc.append("\n");
    doc.append("<abstract> ");
    doc.append(_index_page_lib_list.find(doc_id)->second.abstracts);
    doc.append("\n");
    doc.append("<link> ");
    doc.append(_index_page_lib_list.find(doc_id)->second.link);
    doc.append("\n");
    doc.append("<author> ");
    doc.append(_index_page_lib_list.find(doc_id)->second.author);
    doc.append("\n");
    result.push_back(doc);
  }
  return result;
}

vector<string> WebPageQuery::NoAnswer() {
  vector<string> res;
  res.push_back("No Answer.");
  return res;
}

unordered_map<string, uint64_t> WebPageQuery::Cut(const string &sought) {
  unordered_map<string, uint64_t> result;
  _spilt_tool.Cut(sought, result, true);
  return result;
}
} // namespace hh