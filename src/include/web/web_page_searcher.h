/**
 * @file    :web_page_searcher.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-11
 */

#ifndef __WEB_PAGE_SEARCHER_H__
#define __WEB_PAGE_SEARCHER_H__

#include "cache/redis.h"
#include "common/configuration.h"
#include "common/logger.h"
#include "reactor/event_loop.h"
#include "reactor/tcp_connection.h"
#include "web/web_page_query.h"

namespace hh {
using TcpConnectionPtr = shared_ptr<TcpConnection>;

class WebPageSearcher {
public:
  explicit WebPageSearcher(string sought, TcpConnectionPtr conn,
                           const PageLibPreprocessor &pre, Logger &logger,
                           IConfiguration &conf, RedisCache &cache);
  string DoQuery() const;
  void Process() const {
    // 在process函数中去进行真正的业务逻辑的处理
    std::cout << "sought in process:" << _sought << '\n';
    LOG_INFO("{} search sought: {}", _tcp_connection->GetPeerAddrString(),
             _sought);
    // msg 是要发送给client的内容
    string result = _cache.DoQuery(_sought, 0);
    if (result.empty()) {
      result = DoQuery();
      _cache.AddData(_sought, result, 0);
    }
    _tcp_connection->SendInLoop(result);
  }

private:
  string _sought;
  TcpConnectionPtr _tcp_connection;
  const PageLibPreprocessor &_pre;
  Logger &_logger;
  IConfiguration &_config;
  RedisCache &_cache;
};

} // namespace hh

#endif // WEB_PAGE_SEARCHER_H