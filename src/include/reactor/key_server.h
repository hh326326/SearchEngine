/**
 * @file    :key_server.h
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
#include "reactor/event_loop.h"
#include "reactor/tcp_server.h"
#include "reactor/thread_pool.h"
#include <iostream>
#include <unistd.h>

namespace hh {
using std::cout;
using std::endl;
using std::string;

class MyTask {
public:
  MyTask(string &msg, const TcpConnectionPtr &con, KeyRecommander &kr,
         Logger &logger, RedisCache &cache)
      : _msg(std::move(msg)), _con(std::move(con)), _kr(kr), _logger(logger),
        _cache(cache) {}
  //该函数在线程池中执行的
  // void Process()
  void Process() const {
    //在process函数中去进行真正的业务逻辑的处理
    // std::cout << "msg: in process" << _msg << std::endl;

    LOG_INFO("{} key word query: {}", _con->GetPeerAddrString(), _msg);

    // msg是要发送给client的内容
    string result = _cache.DoQuery(_msg, 1);
    if (result.empty()) {
      _kr.Query(_msg);
      result.append((">>>>>\n")).append(_kr.GetString()).append("<<<<<\n\n");
      _cache.AddData(_msg, result, 1);
    }
    _con->SendInLoop(result);

    // // msg是要发送给client的内容
    // _kr.Query( _msg);
    // string msg;
    // msg.append((">>>>>\n")).append(_kr.GetString()).append("<<<<<\n\n");
    // _con->SendInLoop(msg);
  }

private:
  string _msg;
  TcpConnectionPtr _con;
  KeyRecommander &_kr;
  Logger &_logger;
  RedisCache &_cache;
};

class KeyServer {
public:
  KeyServer(size_t threadNum, size_t queSize, const string &ip,
            unsigned short port, Logger &logger, IConfiguration &config,
            Dictionary &dict, KeyRecommander &kr, RedisCache &cache)
      : _pool(threadNum, queSize), _server(ip, port), _logger(logger),
        _config(config), _dict(dict), _kr(kr), _cache(cache) {}

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
    cout << con->ToString() << " has connected!" << endl;
    string logmsg;
    logmsg = con->ToString() + " has connected!";
    LOG_INFO("Connection message: {}", logmsg);
  }

  void OnMessage(const TcpConnectionPtr &con) {
    //回显
    string msg = con->Receive();
    msg.erase(msg.size() - 1, 1);
    cout << "recv msg  " << msg << endl;
    {
      // 服务器显示的信息
    }
    //接收与发送之间，消息msg是没有做任何处理的 by lili

    MyTask task(msg, con, _kr, _logger, _cache);

    /* gThreadPool->addTask(std::bind(&MyTask::process, &task)); */
    // _pool.addTask(std::bind(&MyTask::Process, task));
    _pool.AddTask(
        [task] { task.Process(); }); //?QUES: 为什么要把`Process()`设置成const
  }

  /*static*/ void OnClose(const TcpConnectionPtr &con) {
    cout << con->ToString() << " has closed!" << endl;
    string logmsg;
    logmsg = con->ToString() + " has closed!";
    LOG_INFO("Close message: {}", logmsg);
  }

private:
  ThreadPool _pool;
  TcpServer _server;
  Logger &_logger;
  IConfiguration &_config;
  Dictionary &_dict;
  KeyRecommander &_kr;
  RedisCache &_cache;
};
}; // namespace hh

#endif
