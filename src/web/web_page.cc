/**
 * @file    :web_page.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-15
 */

#include "web/web_page.h"

namespace hh {
WebPage::WebPage(string &doc, RssItem &rss_item)
    : _doc(std::move(doc)), _rss_item(std::move(rss_item)) {
  _top_words.reserve(KTopKNumber);
  _keyword_list.reserve(KTopKNumber);
}

void WebPage::ProcessDoc() { CreateAbstract(); }
// 根据SimHash的结果，生成_top_words和摘要
void WebPage::CreateAbstract() {
  for (const auto &word : _keyword_list) {
    _top_words.push_back(word.first);
    _doc_abstract.append(word.first);
    _doc_abstract.append(" ");
  }
}

uint64_t WebPage::GetDocId() const { return _doc_id; }

string WebPage::GetDoc() const { return _doc; }
vector<pair<string, double>> &WebPage::GetKeywordsList() {
  return _keyword_list;
}

uint64_t WebPage::GetSimhashValue() const { return _simhash_value; }
} // namespace hh