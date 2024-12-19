#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "web/web_page_query.h"

namespace t2x {
using namespace hh; // NOLINT
auto logger = Logger::GetLogger("/home/hh/searchEngine/log/unit_test_2x_4.log");

TEST_CASE("test caculateCosSimilarity") {
  SUBCASE("test1") {
    unordered_map<string, TF_IDF_Weight> map1 = {{"我", 0.5}, {"是", 0.5},
                                                 {"一", 0.5}, {"个", 0.5},
                                                 {"好", 0.5}, {"人", 0.5}};
    unordered_map<string, TF_IDF_Weight> map2 = {{"我", 0.5}, {"是", 0.5},
                                                 {"一", 0.5}, {"个", 0.5},
                                                 {"好", 0.5}, {"人", 0.5}};
    double result = WebPageQuery::CalculateCosSimilarity(map1, map2);
    CHECK(result > 0.9);
  }

  SUBCASE("test2") {
    unordered_map<string, TF_IDF_Weight> map1 = {{"我", 0.5}, {"是", 0.5},
                                                 {"一", 0.5}, {"个", 0.5},
                                                 {"好", 0.5}, {"人", 0.5}};
    unordered_map<string, TF_IDF_Weight> map2 = {
        {"我", 0.00001}, {"是", 0.00001}, {"一", 0.1},
        {"个", 0.1},     {"好", 0.01},    {"人", 0.001}};
    double result = WebPageQuery::CalculateCosSimilarity(map1, map2);
    CHECK(result < 0.7);
  }

  SUBCASE("test2") {
    unordered_map<string, TF_IDF_Weight> map1 = {{"我", 0.5}, {"是", 0.5},
                                                 {"一", 0.5}, {"个", 0.5},
                                                 {"好", 0.5}, {"人", 0.5}};
    unordered_map<string, TF_IDF_Weight> map2 = {
        {"我", 0.000000001}, {"是", 0.000000001}, {"一", 0.000000001},
        {"个", 0.0001},      {"好", 0.00001},     {"人", 0.000001}};
    double result = WebPageQuery::CalculateCosSimilarity(map1, map2);
    CHECK(result < 0.5);
  }
}

TEST_CASE("test GetPagesOrder") {

  unordered_map<string, TF_IDF_Weight> map1 = {{"我", 0.5}, {"是", 0.5},
                                               {"一", 0.5}, {"个", 0.5},
                                               {"好", 0.5}, {"人", 0.5}};
  unordered_map<string, TF_IDF_Weight> map2 = {{"我", 0.00001}, {"是", 0.00001},
                                               {"一", 0.1},     {"个", 0.1},
                                               {"好", 0.01},    {"人", 0.001}};
  DocsVector vecs;
  vecs.emplace_back(2, map2);
  vecs.emplace_back(1, map1);
  vector<DocId> result = WebPageQuery::GetPagesOrder(map1, vecs);
  for (const auto &doc : result) {
    logger->info("{}", doc);
  }
}
} // namespace t2x