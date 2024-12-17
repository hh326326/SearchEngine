/**
 * @file    :web_page.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */

#ifndef __WEB_PAGE_H__
#define __WEB_PAGE_H__

#include "common/configuration.h"
#include "simhash/Simhasher.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hh {

using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

struct RssItem {
  string title;
  string link;
  string author;
  string description;
};

class WebPage {
public:
  WebPage() = default;
  WebPage(string &doc, RssItem &rss_item);

  void ProcessDoc();
  // 根据SimHash的结果，生成_top_words和摘要
  void CreateAbstract();

  uint64_t GetDocId() const;
  string GetDoc() const;
  vector<pair<string, double>> &GetKeywordsList();
  uint64_t GetSimhashValue() const;
  void SaveFmtString();

  // 对文档按doc_id排序
  // friend bool operator==(const WebPage& lhs, const WebPage& rhs);
  // friend bool operator<(const WebPage& lhs, const WebPage& rhs);

  // 整篇文档，包含xml
  string _doc;
  // 格式化后的网页内容(标题，网址，作者，描述)
  RssItem _rss_item;
  // 自动生成的文档摘要
  string _doc_abstract;
  uint64_t _doc_id;
  // 词频最高的词
  vector<string> _top_words;
  // simhash生成的关键词序列
  vector<pair<string, double>> _keyword_list;
  uint64_t _simhash_value; // 指纹
  // 保存文档的所有词语和词频(不包括停用词)
  unordered_map<string, uint64_t> _term_frequencies;
  const static int KTopKNumber = 20;

private:
  // [[maybe_unused]] Logger& _logger;
  // [[maybe_unused]] IConfiguration& _config;
};

} // namespace hh

#endif // WEB_PAGE_H