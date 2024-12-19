/**
 * @file    :client.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#include "cache/ProtocolParser.h"
#include "client/client_connect.h"
#include <iostream>
#include <string.h>
#include <vector>

using std::cout;
using std::endl;
using std::string;


int main() {
  hh::ClientConnect connect("127.0.0.1", 8888);
  // system("clear");
  puts("\033[1;35m===============SearchEngine_hh================ \033[m ");

  while (true) {
    cout << "请输入类型: 1.关键字推荐 2.网页查询" << endl;
    cout << "格式: "
         << "类型:关键词" << '\n';
    cout << "按9退出" << '\n';

    printf("\033[1;34m[@Searching:] \033[m ");
    //读取输入内容
    string msg;
    getline(std::cin, msg);
    if (msg == "9") {
      cout << "欢迎下次使用\"hh-searchEngine\". " << '\n';
      break;
    }
    char option = msg[0];
    string key = msg.substr(2);
    hh::json js;
    if (option == '1') {
      js["cmd"] = hh::KeyRecommand;
      js["key"] = key;
    } else {
      js["cmd"] = hh::WebPageSearch;
      js["key"] = key;
    }

    //将传输的数据末尾加上\n，来适应服务端
    string sendmsg = hh::ProtocolParser::Json2String(js);
    sendmsg += '\n';

    // cout << sendmsg << '\n';
    connect.sendMessage(sendmsg);

    string retMsg = connect.recvMessage();
    // cout << retMsg << '\n';
    js = hh::json::parse(retMsg);
    std::vector<string> results;
    hh::ProtocolParser::Json2Vector(js, results);
    cout << ">>>>>\n";
    for (const auto line : results) {
      cout << line << '\n';
    }
    cout << "<<<<<\n\n";
  }
}