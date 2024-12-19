/**
 * @file    :rss_parser.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-15
 */

#include "web/rss_parser.h"
#include <regex>

namespace hh {

RssParser::RssParser() {
  _rss.reserve(1000); 
}

void RssParser::Load(XMLDocument &doc) {
  XMLElement *channel =
      doc.FirstChildElement("rss")->FirstChildElement("channel");
  if (channel != nullptr) {
    XMLElement *item = channel->FirstChildElement("item");
    while (item) {
      _rss.emplace_back();

      // std::string title = item->FirstChildElement("title")->GetText();
      // std::string url = item->FirstChildElement("link")->GetText();
      // std::string auther = item->FirstChildElement("author")->GetText();
      // std::string content =
      //     item->FirstChildElement("content")
      //         ? item->FirstChildElement("content")->GetText()
      //     : item->FirstChildElement("description")
      //         ? item->FirstChildElement("description")->GetText()
      //         : "";
      std::regex reg("( |　|&nbsp;|[\r]|[\n]|<[^>]+>|showPlayer[(]+[^)]+[)];)");
      // title = std::regex_replace(title, reg, "");
      // content = std::regex_replace(content, reg, "");
      // if (!title.empty()) {
      //   _rss.back().title = title;
      // }
      // if (!url.empty()) {
      //   _rss.back().link = url;
      // }
      // if (!auther.empty()) {
      //   _rss.back().author = auther;
      // }
      // if (!content.empty()) {
      //   _rss.back().description = content;
      // }
      XMLElement *title = item->FirstChildElement("title");
      if (title != nullptr) {
        _rss.back().title = std::regex_replace(title->GetText(), reg, "");
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
        _rss.back().description = std::regex_replace(description->GetText(), reg, "");;
        // _rss.back().description = FormatText(description->GetText());
      }

      // Next item
      item = item->NextSiblingElement("item");
    }
  }
}

vector<RssItem> &RssParser::GetRssItems() { return _rss; }

// FOR DEBUG
void RssParser::Output(const string &filename) {
  auto logger = Logger::GetLogger();
  std::ofstream ofs(filename);
  logger->info("{}", filename);
  if (ofs.is_open()) {
    logger->info("open success\n");
    size_t docid = 0;
    for (const auto &item : _rss) {
      ofs << "<doc>";
      ofs << "<docid>" << ++docid << "</docid>";
      ofs << "<title>" << item.title << "</title>";
      ofs << "<link>" << item.link << "</link>";
      ofs << "<author>" << item.author << "</author>";
      ofs << "<description>" << item.description << "</description>";
      ofs << "</doc>\n";
    }
  } else {
    logger->info("open failed\n");
  }
}

int LoadXML(const string &filename, XMLDocument &doc) {
  doc.LoadFile(filename.data());
  return doc.ErrorID();
}

} // namespace hh