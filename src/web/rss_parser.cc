/**
 * @file    :rss_parser.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-15
 */

#include "web/rss_parser.h"

namespace hh {
RssParser::RssParser(Logger &logger) : _logger(logger) { _rss.reserve(1000); }

void RssParser::Load(XMLDocument &doc) {
  XMLElement *channel =
      doc.FirstChildElement("rss")->FirstChildElement("channel");
  if (channel != nullptr) {
    XMLElement *item = channel->FirstChildElement("item");
    while (item) {
      _rss.emplace_back();

      XMLElement *title = item->FirstChildElement("title");
      if (title != nullptr) {
        _rss.back().title = title->GetText();
      }

      XMLElement *link = item->FirstChildElement("link");
      if (link != nullptr) {
        _rss.back().link = link->GetText();
      }

      XMLElement *author = item->FirstChildElement("author");
      if (author != nullptr) {
        _rss.back().author = author->GetText();
      }

      XMLElement *description = item->FirstChildElement("description");
      if (description != nullptr) {
        _rss.back().description = FormatText(description->GetText());
      }

      // Next item
      item = item->NextSiblingElement("item");
    }
  }
}

vector<RssItem> &RssParser::GetRssItems() { return _rss; }

// FOR DEBUG
void RssParser::Output(const string &filename) {
  std::ofstream ofs(filename);
  LOG_INFO("{}", filename);
  if (ofs.is_open()) {
    LOG_INFO("open success\n");
    size_t docid = 0;
    for (const auto &item : _rss) {
      // fmt::print(ofs, "<doc>\n");
      // fmt::print(ofs, "\t<docid>{}</docid>\n", ++docid);
      // fmt::print(ofs, "\t<title>{}</title>\n", item.title);
      // fmt::print(ofs, "\t<link>{}</link>\n", item.link);
      // fmt::print(ofs, "\t<author>{}</author>\n", item.author);
      // fmt::print(ofs, "\t<description>{}</description>\n", item.description);
      // fmt::print(ofs, "</doc>\n");
      // ofs << "<doc>\n";
      // ofs << "\t<docid>" << ++docid << "</docid>\n";
      // ofs << "\t<title>" << item.title << "</title>\n";
      // ofs << "\t<link>" << item.link << "</link>\n";
      // ofs << "\t<author>" << item.author << "</author>\n";
      // ofs << "\t<description>" << item.description << "</description>\n";
      // ofs << "</doc>\n";
      ofs << "<doc>";
      ofs << "<docid>" << ++docid << "</docid>";
      ofs << "<title>" << item.title << "</title>";
      ofs << "<link>" << item.link << "</link>";
      ofs << "<author>" << item.author << "</author>";
      ofs << "<description>" << item.description << "</description>";
      ofs << "</doc>\n";
    }
  } else {
    LOG_ERROR("open failed\n");
  }
}

int LoadXML(const string &filename, XMLDocument &doc) {
  doc.LoadFile(filename.data());
  return doc.ErrorID();
}

} // namespace hh