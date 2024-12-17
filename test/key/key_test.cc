#include "common/configuration.h"
#include "common/logger.h"
#include "key/dict_producer.h"
#include "key/dictionary.h"
#include "key/key_recommander.h"
#include "key/myTimer.h"

#include <string>

using std::string;

int main() {
  hh::MyTimer mytimer(1);
  mytimer.start();

  hh::Logger logger("/home/hh/searchEngine/log/unit_test_hh.log");
  hh::Configuration config("/home/hh/searchEngine/conf/configure.json", logger);

  mytimer.stop();
  mytimer.printTime();

  mytimer.start();
  hh::Dictionary dict(logger, config);
  mytimer.stop();
  mytimer.printTime();

  mytimer.start();
  hh::KeyRecommander kr(dict);
  // kr.Query("hello");
  // kr.Print();

  // kr.Query("apple凑合");
  // kr.Print();

  // kr.Query("凑合");
  // kr.Print();

  kr.Query("你好");
  kr.Print();

  kr.Query("whh");
  kr.Print();

  // kr.Query("你好apple");
  // kr.Print();

  // kr.Query("凑合apple");
  // kr.Print();

  mytimer.stop();
  mytimer.printTime();
  std::cout << "key test done" << std::endl;
  return 0;
}

// int main()
// {
//     string logpath("../../log/jifenghi.log");
//     string confpath("../../conf/configure.json");

//     hh::Logger logger(logpath);
//     hh::Configuration config(confpath, logger);

//     hh::Dictionary dict(config, logger);

//     hh::KeyRecommander kr("apple", dict);

//     kr.Query();
//     kr.Print();
//     return 0;
// }