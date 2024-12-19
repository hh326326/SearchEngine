#include "common/configuration.h"
#include "common/logger.h"
#include "common/split_tool.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
  auto logger = hh::Logger::GetLogger("./log/unit_test_hh.log");
  hh::Configuration config("./conf/configure.json");
  hh::SplitTool sp(config);
  string str("国家统计局局长就2020年全年国民经济运行情况答记者问");
  hh::unordered_map<string, uint64_t> wordsmap;
  sp.Cut(str, wordsmap, true);

  for (const auto &elem : wordsmap) {
    cout << elem.first << " : " << elem.second << endl;
  }
  return 0;
}