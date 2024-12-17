/**
 * @file    :search_engine.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-12
 */
#ifndef __MODULEONESERVER_H__
#define __MODULEONESERVER_H__

#include "cache/redis.h"
#include "common/configuration.h"
#include "common/logger.h"
#include "key/dict_producer.h"
#include "key/dictionary.h"
#include "key/key_recommander.h"
#include "key/myTimer.h"
#include "reactor/tcp_server.h"
#include "reactor/thread_pool.h"
#include "web/web_page_searcher.h"
#include <iostream>
#include <unistd.h>

namespace hh {
using std::cout;
using std::endl;
using std::string;

struct Setting {
  Setting(Dictionary &dict, KeyRecommander &kr, PageLib &pl,
          PageLibPreprocessor &plp, RedisCache &cache)
      : _dict(dict), _key_recommander(kr), _plpage_lib(pl),
        _page_lib_preprocessor(plp), cache(cache) {
    _plpage_lib.Load();
    _page_lib_preprocessor.CreatePageLibList();
    _page_lib_preprocessor.RmRedundantPages();
    _page_lib_preprocessor.Cut();
    _page_lib_preprocessor.BuildInvertIndexTable();
    _page_lib_preprocessor.LoadIndexPageList();
  }

  PageLib &_plpage_lib;
  Dictionary &_dict;
  KeyRecommander &_key_recommander;
  PageLibPreprocessor &_page_lib_preprocessor;
  RedisCache &cache;
};

class MyTask {
public:
  MyTask(string &msg, const TcpConnectionPtr &con,
         KeyRecommander &key_recommander, RedisCache &cache, Logger &logger)
      : _msg(std::move(msg)), _con(std::move(con)), _kr(key_recommander),
        _logger(logger), _cache(cache) {}
  //该函数在线程池中执行的
  // void Process()
  void Process() const {
    // 关键词推荐
    //在process函数中去进行真正的业务逻辑的处理

    string result = _cache.DoQuery(_msg, 1);
    LOG_INFO("{} key word query: {} in process", _con->GetPeerAddrString(),
             _msg);
    if (result.empty()) {
      _kr.Query(_msg);
      result.append((">>>>>\n")).append(_kr.GetString()).append("<<<<<\n\n");
      _cache.AddData(_msg, result, 1);
    }

    _con->SendInLoop(result);
  }

private:
  string _msg;
  TcpConnectionPtr _con;
  Logger &_logger;
  KeyRecommander &_kr;
  RedisCache &_cache;
};

class WebServer {
public:
  WebServer(size_t threadNum, size_t queSize, const string &ip,
            unsigned short port, Logger &logger, IConfiguration &config,
            Setting &setting, RedisCache &cache)
      : _pool(threadNum, queSize), _server(ip, port), _logger(logger),
        _config(config), _settings(setting), _cache(cache) {}

  void Start() {
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
    LOG_INFO("我在OnConnection()");
    cout << con->ToString() << " has connected!" << endl;
    string logmsg{con->ToString() + " has connected!"};
    LOG_INFO("Connection message: {}", logmsg);
  }

  void OnMessage(const TcpConnectionPtr &con) {
    //回显
    LOG_INFO("我在Onmessage()");
    string msg = con->Receive();
    msg.erase(msg.size() - 1, 1);
    cout << "recv msg  " << msg << endl;
    {
      // 服务器显示的信息
    }
    //接收与发送之间，消息msg是没有做任何处理的 by lili
    char type = msg[0];
    msg = msg.substr(2);
    cout << "new mesg = " << msg << '\n';
    if (type == '1') {
      // 关键词推荐
      MyTask task(msg, con, _settings._key_recommander, _cache, _logger);
      _pool.AddTask(std::bind(&MyTask::Process, task));
    } else {
      // 网页查询
      WebPageSearcher task(msg, con, _settings._page_lib_preprocessor, _logger,
                           _config, _settings.cache);
      _pool.AddTask(
          [task] { task.Process(); }); //?QUES: 为什么要把`Process()`设置成const
    }

    /* gThreadPool->addTask(std::bind(&MyTask::process, &task)); */
    // _pool.AddTask(std::bind(&MyTask::Process, task));
    // _pool.AddTask(
    //     [task] { task.Process(); });  //?QUES:
    //     为什么要把`Process()`设置成const
  }

  /*static*/ void OnClose(const TcpConnectionPtr &con) {
    cout << con->ToString() << " has closed!" << endl;
    string logmsg{con->ToString() + " has closed!"};
    LOG_INFO("Close message: {}", logmsg);
  }

private:
  ThreadPool _pool;
  TcpServer _server;
  Logger &_logger;
  IConfiguration &_config;
  Setting &_settings;
  RedisCache &_cache;
};
}; // namespace hh

#endif