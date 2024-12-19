/**
 * @file    :page_lib.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-15
 */

#include "web/page_lib.h"
#include "web/rss_parser.h"
#include <cstdint>
#include <fstream>
#include <ios>
#include <string>
#include <utility>

namespace hh {
using std::ios;
using std::make_pair;
using std::ofstream;

PageLib::PageLib( IConfiguration &config)
    :  _config(config), _dir_scanner( _config),
      _rss_parser(),
      _simhasher(_config.GetConfig()["jieba.dict"],
                 _config.GetConfig()["hmm_model"], _config.GetConfig()["idf"],
                 _config.GetConfig()["stop_words"]) {
  _ripe_pages.reserve(5000);
}

// 从文件中加载网页库
void PageLib::Load() {
  int docid = 0;
  _dir_scanner();
  vector<string> files = _dir_scanner.GetFilesPath();
  for (const string &file : files) {
    XMLDocument doc;
    LoadXML(file, doc);
    _rss_parser.Load(doc);
  }
  for (const RssItem &rss_item : _rss_parser.GetRssItems()) {
    string page;
    page.append("<doc>");
    page.append("<docid>");
    page.append(std::to_string(++docid));
    page.append("</docid>");
    page.append("<title>");
    page.append(rss_item.title);
    page.append("</title>");
    page.append("<link>");
    page.append(rss_item.link);
    page.append("</link>");
    page.append("<author>");
    page.append(rss_item.author);
    page.append("</author>");
    page.append("<description>");
    page.append(rss_item.description);
    page.append("</description>");
    page.append("</doc>");
    // std::cout << page << '\n';
    _ripe_pages.emplace_back(page);
  }
}
// 获取网页库中的网页数量
size_t PageLib::GetPageNum() const { return _ripe_pages.size(); }

vector<RssItem> &PageLib::GetRssItems() { return _rss_parser.GetRssItems(); }

vector<string> &PageLib::GetRipePages() { return _ripe_pages; }

Simhasher &PageLib::GetSimhaser() { return _simhasher; }

// 离线存储网页库 'ripe_page.dat'和位置偏移库offset_lib.dat'
void PageLib::StoreOnDisk() {
  // 离线存储网页库
  ofstream ofs(_config.GetConfig()["ripepage"],
               ios::binary);

  for (const string &page : _ripe_pages) {
    ofs.write(page.c_str(), static_cast<int64_t>(page.size()));
  }
  ofs.close();
  // 存储位置偏移库
  ofstream ofs_offset(_config.GetConfig()["offset"]
                      );

  uint64_t doc_id = 1;
  uint64_t offset = 0;
  uint64_t doc_size = 0;
  for (const string &page : _ripe_pages) {
    // ofs_offset.write(reinterpret_cast<const char *>(&doc_id),
    // sizeof(uint64_t)); ofs_offset.write(reinterpret_cast<const char
    // *>(&offset), sizeof(uint64_t));
    ofs_offset << doc_id << " " << std::to_string(offset) << " ";
    offset += page.size();

    doc_size = _ripe_pages[doc_id - 1].size();
    // ofs_offset.write(reinterpret_cast<const char *>(&doc_size),
    //  sizeof(uint64_t));
    ofs_offset << std::to_string(doc_size) << '\n';
    ++doc_id;
    _offset_lib.insert(make_pair(doc_id, make_pair(offset, doc_size)));
  }
  ofs_offset.close();
}

} // namespace hh