/**
 * @file    :search_engine.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __MODULEONESERVER_H__
#define __MODULEONESERVER_H__

#include "cache/cache_manager.h"
#include "cache/redis.h"
#include "common/configuration.h"
#include "common/logger.h"
#include "key/dict_producer.h"
#include "key/dictionary.h"
#include "key/key_query.h"
#include "key/key_recommander.h"
#include "key/myTimer.h"
#include "reactor/tcp_server.h"
#include "reactor/thread_pool.h"
#include "web/web_page_searcher.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <utility>
#include "cache/ProtocolParser.h"
#include "reactor/settion_manager.h"


namespace hh {
using std::cout;
using std::endl;
using std::string;



class WebServer {
public:
  WebServer(size_t threadNum, size_t queSize, const string &ip,
            unsigned short port, IConfiguration &config,
            Setting &setting)
      : _pool(threadNum, queSize), _server(ip, port),
        _config(config), _settings(setting){}

  void Start() {
    CacheManager::GetCacheManager();
    _pool.Start(); //计算线程全部启动
    // using namespace std::placeholders;
    // _server.setAllCallback(bind(&EchoServer::OnConnection, this, _1)
    //                        , bind(&EchoServer::OnMessage, this, _1)
    //                        , bind(&EchoServer::OnClose, this, _1));
    _server.SetAllCallback(
        // use for logger in OnConnection
        [this](auto &&PH1) { OnConnection(std::forward<decltype(PH1)>(PH1)); },
        [this](auto &&PH1) { OnMessage(std::forward<decltype(PH1)>(PH1)); },
        // use for logger in OnClose
        [this](auto &&PH1) { OnClose(std::forward<decltype(PH1)>(PH1)); });
    _server.Start(); // IO线程启动
  }

  void stop() {
    _pool.Stop();
    _server.Stop();
  }

  /*static*/ void OnConnection(const TcpConnectionPtr &con) {
    auto logger = Logger::GetLogger(); // 获取日志实例
    logger->info("我在OnConnection()");
    cout << con->ToString() << " has connected!" << endl;
    string logmsg{con->ToString() + " has connected!"};
    logger->info("Connection message: {}", logmsg);
  }

  void OnMessage(const TcpConnectionPtr &con) {
    auto logger = Logger::GetLogger(); // 获取日志实例
    //回显
    logger->info("我在Onmessage()");
    string msg = con->Receive();
    msg.pop_back();
    cout << "recv msg  " << msg << endl;
    {
      // 服务器显示的信息
    }
    //接收与发送之间，消息msg是没有做任何处理的 by lili
    json recv_json = ProtocolParser::DoParse(msg);
    json send_json;
    cout << "recv_json[\"cmd\"]"
         << (int)(recv_json["cmd"]) << '\n';
    cout << "recv_json[\"key\"]" << recv_json["key"] << '\n';

    if ((int)(recv_json["cmd"] == KeyRecommand)){
            KeyRecommander task(recv_json["key"], con, _settings._key_query );
      _pool.AddTask([task] {task.Process();});
    }
    else if ((int)(recv_json["cmd"] == WebPageSearch)) {
      WebPageSearcher task(recv_json["key"], con, 
                           _config);
      _pool.AddTask(
          [task] { task.Process(); }); //?QUES: 为什么要把`Process()`设置成const
      }
  }


    // _pool.AddTask(std::bind(&MyTask::Process, task));
    // _pool.AddTask(
    //     [task] { task.Process(); });  //?QUES:
    //     为什么要把`Process()`设置成const


  /*static*/ void OnClose(const TcpConnectionPtr &con) {
    auto logger = Logger::GetLogger();
    cout << con->ToString() << " has closed!" << endl;
    string logmsg{con->ToString() + " has closed!"};
    logger->error("Close message: {}", logmsg);
  }

private:
  ThreadPool _pool;
  TcpServer _server;
  IConfiguration &_config;
  Setting &_settings;
};
}; // namespace hh

#endif