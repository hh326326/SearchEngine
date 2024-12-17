#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "web/page_lib_preprocessor.h"
#include "web/web_page.h"
#include <iostream>

namespace t23 {
using namespace hh; // NOLINT
hh::Logger logger("/home/hh/searchEngine/log/unit_test_22.log");
Configuration config("/home/hh/searchEngine/conf/configure.json", logger);
Simhasher simhasher(config.GetConfig()["jieba.dict"],
                    config.GetConfig()["hmm_model"], config.GetConfig()["idf"],
                    config.GetConfig()["stop_words"]);

TEST_CASE("test stl basic") {
  vector<WebPage> web_pages;
  web_pages.reserve(20);
}

TEST_CASE("test  basic") {
  PageLib page_lib(logger, config);
  page_lib.Load();
  REQUIRE(page_lib.GetRipePages().size() > 0);
  PageLibPreprocessor page_lib_preprocessor(page_lib, logger, config);
}

TEST_CASE("test page_test") {
  string doc =
      "日前，一些网约车平台相继推出措施，优化老年人打车服务。1月22日起，滴滴老"
      "年人打车小程序上线试运行，60岁以上老年人可在亲友协助下注册、认证，并设定"
      "10个常用地址。1月25日起，滴滴还在一些城市开通电话叫车热线，乘客拨打后由"
      "平台代为叫车。高德打车近日也上线了助老打车系列服务。通过高德打车小程序“"
      "一键叫车”功能，老年乘客不再需要输入起点和终点即可呼叫出租车，上车点根据"
      "乘客所在位置自动推荐，车辆到达后，乘客可当面与司机沟通目的地。    "
      "交通运输部表示，将指导鼓励网约车平台优化约车软件、降低操作难度，让老年人"
      "能用、会用、想用，同时保持电召、扬召服务，保证老年人不会上网或者没有手机"
      "也能打车";
  RssItem rss_item{
      "网约车平台优化老年人打车服务",
      "http://auto.people.com.cn/n1/2021/0203/c1005-32020974.html", "人民网",
      "日前，一些网约车平台相继推出措施，优化老年人打车服务。1月22日起，滴滴老"
      "年人打车小程序上线试运行，60岁以上老年人可在亲友协助下注册、认证，并设定"
      "10个常用地址。1月25日起，滴滴还在一些城市开通电话叫车热线，乘客拨打后由"
      "平台代为叫车。高德打车近日也上线了助老打车系列服务。通过高德打车小程序“"
      "一键叫车”功能，老年乘客不再需要输入起点和终点即可呼叫出租车，上车点根据"
      "乘客所在位置自动推荐，车辆到达后，乘客可当面与司机沟通目的地。    "
      "交通运输部表示，将指导鼓励网约车平台优化约车软件、降低操作难度，让老年人"
      "能用、会用、想用，同时保持电召、扬召服务，保证老年人不会上网或者没有手机"
      "也能打车"};
  WebPage page(doc, rss_item);
  page.ProcessDoc();
  std::cout << page.GetKeywordsList() << std::endl;
  std::cout << page.GetSimhashValue() << std::endl;

  // SUBCASE("test page_test twice") {
  string doc2 =
      "日前，一些网约车平台相继推出措施，优化老年人打车服务。1月22日起，滴滴老"
      "年人打车小程序上线试运行，60岁以上老年人可在亲友协助下注册、认证，并设定"
      "10个常用地址。1月25日起，滴滴还在一些城市开通电话叫车热线，乘客拨打后由"
      "平台代为叫车。高德打车近日也上线了助老打车系列服务。通过高德打车小程序“"
      "一键叫车”功能，老年乘客不再需要输入起点和终点即可呼叫出租车，上车点根据"
      "乘客所在位置自动推荐，车辆到达后，乘客可当面与司机沟通目的地。    "
      "交通运输部表示，将指导鼓励网约车平台优化约车软件、降低操作难度，让老年人"
      "能用、会用、想用，同时保持电召、扬召服务，保证老年人不会上网或者没有手机"
      "也能打车";
  RssItem rss_item2{
      "网约车平台优化老年人打车服务",
      "http://auto.people.com.cn/n1/2021/0203/c1005-32020974.html", "人民网",
      "日前，一些网约车平台相继推出措施，优化老年人打车服务。1月22日起，滴滴老"
      "年人打车小程序上线试运行，60岁以上老年人可在亲友协助下注册、认证，并设定"
      "10个常用地址。1月25日起，滴滴还在一些城市开通电话叫车热线，乘客拨打后由"
      "平台代为叫车。高德打车近日也上线了助老打车系列服务。通过高德打车小程序“"
      "一键叫车”功能，老年乘客不再需要输入起点和终点即可呼叫出租车，上车点根据"
      "乘客所在位置自动推荐，车辆到达后，乘客可当面与司机沟通目的地。    "
      "交通运输部表示，将指导鼓励网约车平台优化约车软件、降低操作难度，让老年人"
      "能用、会用、想用，同时保持电召、扬召服务，保证老年人不会上网或者没有手机"
      "也能打车"};
  WebPage page2(doc2, rss_item2);
  page2.ProcessDoc();
  std::cout << page2.GetKeywordsList() << std::endl;
  std::cout << page2.GetSimhashValue() << std::endl;
  CHECK((simhash::Simhasher::isEqual(page.GetSimhashValue(),
                                     page2.GetSimhashValue())) == 0);
  // }
}

// TEST_CASE("test page_test 3") {
//   string doc =
//   "日前，一些网约车平台相继推出措施，优化老年人打车服务。1月22日起，滴滴老年人打车小程序上线试运行，60岁以上老年人可在亲友协助下注册、认证，并设定10个常用地址。1月25日起，滴滴还在一些城市开通电话叫车热线，乘客拨打后由平台代为叫车。高德打车近日也上线了助老打车系列服务。通过高德打车小程序“一键叫车”功能，老年乘客不再需要输入起点和终点即可呼叫出租车，上车点根据乘客所在位置自动推荐，车辆到达后，乘客可当面与司机沟通目的地。
//   交通运输部表示，将指导鼓励网约车平台优化约车软件、降低操作难度，让老年人能用、会用、想用，同时保持电召、扬召服务，保证老年人不会上网或者没有手机也能打车";
//   RssItem rss_item{
//       "网约车平台优化老年人打车服务",
//       "http://auto.people.com.cn/n1/2021/0203/c1005-32020974.html", "人民网",
//       "日前，一些网约车平台相继推出措施，优化老年人打车服务。1月22日起，滴滴老"
//       "年人打车小程序上线试运行，60岁以上老年人可在亲友协助下注册、认证，并设定"
//       "10个常用地址。1月25日起，滴滴还在一些城市开通电话叫车热线，乘客拨打后由"
//       "平台代为叫车。高德打车近日也上线了助老打车系列服务。通过高德打车小程序“"
//       "一键叫车”功能，老年乘客不再需要输入起点和终点即可呼叫出租车，上车点根据"
//       "乘客所在位置自动推荐，车辆到达后，乘客可当面与司机沟通目的地。    "
//       "交通运输部表示，将指导鼓励网约车平台优化约车软件、降低操作难度，让老年人"
//       "能用、会用、想用，同时保持电召、扬召服务，保证老年人不会上网或者没有手机"
//       "也能打车"};
//   WebPage page(doc, rss_item, logger, config);
//   page.ProcessDoc();
//   std::cout << page.GetKeywordsList() << std::endl;
//   std::cout << page.GetSimhashValue() << std::endl;

//   // SUBCASE("test page_test twice") {
//   string doc2 =
//   "日前，一些网约车平台x施，优化老年人打车服务。1月22日起，滴滴老年人打车小程序上线试运行，60岁以上老年人可在亲友协助下注册、认证，并设定1x址。1月25日起，滴滴还在一些城市开通电话叫车热线，乘客拨打后由平台代为叫车。高德打车近日也上线了助老打车系列服务。通过高德打车小程序“一键叫车”功能，老年乘客不再需要输入起点和终点即可呼叫出租车，上车点根据乘客所在位置自动推荐，车辆到达后，乘客可当面与司机沟通目的地。
//   交通运输部表示，将指导鼓励网约车平台优化约车软件、降低操作难度，让老年人能用、会用、想用，同时保持电召、扬召服务，保证老年人不会上网或者没有手机也能打车";
//   RssItem rss_item2{
//       "网约车平台优化老年人打车服务",
//       "http://auto.people.com.cn/n1/2021/0203/c1005-32020974.html", "人民网",
//       "日前，一些网约车x施，优化老年人打车服务。1月22日起，滴滴老"
//       "年人打车小程序上线试运行，60岁以x助下注册、认证，并设定"
//       "10个常用地址。1月25日起，滴滴还在一些城市开通电话叫车热线，乘客拨打后由"
//       "平台代为叫车。高德打车近日也上线了助老打车系列服务。通过高德打车小程序“"
//       "一键叫车”功能，老年乘客不再需要输入起点即可呼叫出租车，上车点根据"
//       "乘客所在位置自动推荐，车辆到达后，乘客可当面与司机沟通目的地。    "
//       "交通运输部表示，将指导鼓励网约车平台优化约车软件、降低操作难度，让老年人"
//       "能用、会用、想用，同时保持电召、扬召服务，保证老年人不会上网或者没有手机"
//       "也能打车"};
//   WebPage page2(doc2, rss_item2, logger, config);
//   page2.ProcessDoc();
//   std::cout << page2.GetKeywordsList() << std::endl;
//   std::cout << page2.GetSimhashValue() << std::endl;
//   CHECK((simhash::Simhasher::isEqual(page.GetSimhashValue(),
//                                      page2.GetSimhashValue())) == 0);
//   // }
// }
} // namespace t23