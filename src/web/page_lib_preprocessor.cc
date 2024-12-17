/**
 * @file    :page_lib_preprocessor.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-15
 */

#include "web/page_lib_preprocessor.h"
#include "key/dictionary.h"
#include "web/calculate.h"
#include "web/web_page.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <utility>

namespace hh {
using std::ios;
using std::make_pair;
using std::ofstream;

PageLibPreprocessor::PageLibPreprocessor(PageLib &page_lib, Logger &logger,
                                         IConfiguration &config)
    : _page_lib(page_lib), _logger(logger), _config(config) {}

void PageLibPreprocessor::Do() {
  CreatePageLibList();
  RmRedundantPages();
  Cut();
  BuildInvertIndexTable();
  // TODO(hh): tmp use
  LoadIndexPageList();
}

// 创建PageLibList后销毁PageLib对象  --->shared_ptr
void PageLibPreprocessor::CreatePageLibList() {
  size_t list_size = _page_lib.GetPageNum();
  _page_lib_list.reserve(list_size);

  vector<RssItem> rss_items = _page_lib.GetRssItems();
  vector<string> ripe_pages = _page_lib.GetRipePages();

  for (size_t i = 0; i < list_size; ++i) {
    _page_lib_list.emplace_back(ripe_pages[i], rss_items[i]);
    _page_lib_list.at(i)._doc_id = i + 1;
    // 根据SimHash的结果，生成_top_words(关键词)和摘要_doc_abstract
    SimHash(_page_lib_list.at(i));
    _page_lib_list.at(i).ProcessDoc();
  }
  rss_items.clear();
  ripe_pages.clear();
}

void PageLibPreprocessor::Cut() {
  SplitTool split_tool(_logger, _config);
  for (auto &page : _page_lib_list) {
    split_tool.Cut(page._rss_item.description, page._term_frequencies, true);
    for (const auto &pair : page._term_frequencies) {
      _document_frequencies[pair.first] += 1;
    }
  }
}

vector<WebPage> &PageLibPreprocessor::GetPageLibList() {
  return _page_lib_list;
}

void PageLibPreprocessor::RmRedundantPages() {
  for (auto it = _page_lib_list.begin(); it != _page_lib_list.end(); ++it) {
    for (auto pos = it + 1; pos != _page_lib_list.end();) {
      if (simhash::Simhasher::isEqual(it->GetSimhashValue(),
                                      pos->GetSimhashValue())) {
        pos = _page_lib_list.erase(pos);
      } else {
        ++pos;
      }
    }
  }
}

// TODO(hh): 代码复用
void PageLibPreprocessor::BuildInvertIndexTable() {
  uint64_t total_docs_num = _page_lib_list.size();
  _invert_index_table.reserve(_document_frequencies.size());
  // FOR 每篇文档(WebPage)
  for (const auto &page : _page_lib_list) {
    unordered_map<string, TF_IDF_Weight> doc_weights;
    doc_weights.reserve(page._term_frequencies.size());
    // 计算每个单词的权重w
    for (const auto &pair : page._term_frequencies) {
      uint64_t term_frequency = pair.second;
      uint64_t document_frequency = _document_frequencies[pair.first];
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
      _invert_index_table[pair.first].insert(
          make_pair(page._doc_id, pair.second / after_sqrt));
    }
  }
}

// 将预处理后的网页库和网页偏移库存储到磁盘上
void PageLibPreprocessor::StoreOnDisk() {
  // 存储网页库，目标文件夹必须存在
  ofstream ofs("/home/hh/searchEngine/data/database/new_ripe_page.dat",
               ios::binary);
  for (const auto &page : _page_lib_list) {
    ofs.write(page._doc.c_str(), static_cast<int64_t>(page._doc.size()));
  }
  ofs.close();
  // 存储位置偏移库
  ofstream ofs_offset("/home/hh/searchEngine/data/database/new_offset_lib.dat",
                      ios::binary);
  uint64_t offset = 0;
  uint64_t doc_size = 0;
  for (const auto &page : _page_lib_list) {
    uint64_t doc_id = page._doc_id;
    ofs_offset.write(reinterpret_cast<const char *>(&doc_id), sizeof(uint64_t));
    ofs_offset.write(reinterpret_cast<const char *>(&offset), sizeof(uint64_t));
    offset += page._doc.size();

    doc_size = _page_lib_list[doc_id]._doc.size();
    ofs_offset.write(reinterpret_cast<const char *>(&doc_size),
                     sizeof(uint64_t));
    ++doc_id;
    // TODO(hh): consider one more function
    _offset_lib.insert(make_pair(doc_id, make_pair(offset, doc_size)));
  }
  ofs_offset.close();
}

// 使用simHash计算出网页描述的simhash值和关键词序列
void PageLibPreprocessor::SimHash(WebPage &web_page) {
  if (!web_page._rss_item.description.empty()) {
    _page_lib.GetSimhaser().extract(web_page._rss_item.description,
                                    web_page._keyword_list,
                                    hh::WebPage::KTopKNumber);
    _page_lib.GetSimhaser().make(web_page._rss_item.description,
                                 hh::WebPage::KTopKNumber,
                                 web_page._simhash_value);
  }
}

void PageLibPreprocessor::LoadIndexPageList() {
  for (const auto &page : _page_lib_list) {
    _index_page_lib_list.emplace(page._doc_id, page);
  }
}

} // namespace hh