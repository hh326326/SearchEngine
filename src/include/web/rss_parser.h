/**
 * @file    :rss_parser.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-11
 */

#ifndef __RSS_PARSER_H__
#define __RSS_PARSER_H__

#include "common/configuration.h"
#include "tinyxml2/tinyxml2.h"
#include "web/web_page.h"
#include <fstream>
#include <memory>
#include <re2/re2.h>
#include <string>
#include <vector>

namespace hh {
using std::string;
using std::vector;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;

class RssParser {
public:
  explicit RssParser(Logger &logger);
  void Load(XMLDocument &doc);
  vector<RssItem> &GetRssItems();

  static string FormatText(string s) {
    RE2::GlobalReplace(&s, "<[^>]*>", " ");
    return s;
  }

  // FOR DEBUG
  void Output(const string &filename);

private:
  vector<RssItem> _rss;
  // vector<WebPage> _page_lib;
  [[maybe_unused]] Logger &_logger;
};

int LoadXML(const string &filename, XMLDocument &doc);

} // namespace hh
// namespace hh

#endif // RSS_PARSER_H