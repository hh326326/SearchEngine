#include "common/configuration.h"
#include "common/logger.h"
#include "key/dict_producer.h"
#include "key/dictionary.h"
#include "key/key_query.h"
#include "key/myTimer.h"

#include <string>

using std::string;

int main() {
  hh::MyTimer mytimer(1);
  mytimer.start();

  hh::Configuration config("./conf/configure.json");

  mytimer.stop();
  mytimer.printTime();

  mytimer.start();
  hh::Dictionary dict(config);
  mytimer.stop();
  mytimer.printTime();

  mytimer.start();
  hh::KeyQuery kq(dict);
  // kq.Query("hello");
  // kq.Print();

  // kq.Query("apple凑合");
  // kq.Print();

  // kq.Query("凑合");
  // kq.Print();

  kq.Query("你好");
  kq.Print();

  kq.Query("whh");
  kq.Print();

  // kq.Query("你好apple");
  // kq.Print();

  // kq.Query("凑合apple");
  // kq.Print();

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

//     hh::KeyRecommander kq("apple", dict);

//     kq.Query();
//     kq.Print();
//     return 0;
// }