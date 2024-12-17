#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "web/rss_parser.h"

namespace t21 {
using namespace hh; // NOLINT
hh::Logger logger("/home/hh/searchEngine/log/unit_test_21.log");

TEST_CASE("test rss_parser") {
  XMLDocument doc;
  LoadXML("/home/hh/searchEngine/data/raw_data/yuliao/people_daily/auto.xml",
          doc);
  // LoadXML("/home/hh/searchEngine/data/raw_data/yuliao/people_daily/edu.xml",
  // doc);
  RssParser rp(logger);
  rp.Load(doc);
  // auto reslut = seven::FormatText(">大家好<");
  // cout << reslut.get<1>();
  rp.Output("rss.txt");
}

TEST_CASE("test rss_parser::FormatText()") {
  RssParser rp(logger);
  string s = ">大家好<";
  string reslut = hh::RssParser::FormatText(s);
  CHECK(reslut == ">大家好<");
  s = "<p>大家好</p>";
  reslut = hh::RssParser::FormatText(s);
  CHECK(reslut == " 大家好 ");
  s = "<p>大家好</p><p>大家好</p>";
  reslut = hh::RssParser::FormatText(s);
  CHECK(reslut == " 大家好  大家好 ");
}
} // namespace t21