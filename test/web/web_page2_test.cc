#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include "web/web_page.h"
#include <iostream>

namespace t26 {
using namespace hh; // NOLINT
Configuration config("/home/hh/searchEngine/conf/configure.json");

TEST_CASE("test stl basic") {
  string test = "日前，一些网约车平台相继推出措施，优化老年人打车服务。1月22";
  RssItem rss_item2{
      "网约车平台优化老年人打车服务",
      "http://auto.people.com.cn/n1/2021/0203/c1005-32020974.html", "人民网",
      "日前，一些网约车x施，优化老年人打车服务。1月22日起，滴滴老"
      "也能打车"};
  vector<WebPage> web_pages;
  web_pages.reserve(20);
  web_pages.emplace_back(test, rss_item2);
}
} // namespace t26