/**
 * @file    :page_lib_preprocessor.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-14
 */

#ifndef __PAGE_LIB_PREPROCESSOR_H__
#define __PAGE_LIB_PREPROCESSOR_H__

#include "common/configuration.h"
#include "common/split_tool.h"
#include "web/calculate.h"
#include "web/page_lib.h"
#include "web/web_page.h"
#include <cstdint>

namespace hh {

class PageLibPreprocessor {
public:
  PageLibPreprocessor(PageLib &page_lib, Logger &logger,
                      IConfiguration &config);
  void Do();
  // 创建PageLibList后销毁PageLib对象  --->shared_ptr
  void CreatePageLibList();
  void Cut();
  vector<WebPage> &GetPageLibList();
  void RmRedundantPages();
  void BuildInvertIndexTable();
  // 将预处理后的网页库和网页偏移库存储到磁盘上
  void StoreOnDisk();
  // 使用simHash计算出网页描述的simhash值和关键词序列
  void SimHash(WebPage &web_page);

  // TODO(hh) tmp use
  void LoadIndexPageList();

  Logger &_logger;
  IConfiguration &_config;
  PageLib &_page_lib;
  vector<WebPage> _page_lib_list;
  std::unordered_map<DocId, WebPage> _index_page_lib_list;
  // 存放每篇文档的网页库的位置信息
  unordered_map<DocId, pair<uint64_t, uint64_t>> _offset_lib;
  // 存放包含某个单词的文档数
  unordered_map<string, uint64_t> _document_frequencies;
  // 倒排索引表
  unordered_map<string, set<pair<DocId, TF_IDF_Weight>>> _invert_index_table;
};

#endif // PAGE_LIB_PREPROCESSOR_H

} // namespace hh