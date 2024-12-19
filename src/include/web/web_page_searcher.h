/**
 * @file    :web_page_searcher.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-11
 */

#ifndef __WEB_PAGE_SEARCHER_H__
#define __WEB_PAGE_SEARCHER_H__

#include "cache/ProtocolParser.h"
#include "cache/redis.h"
#include "common/configuration.h"
#include "common/logger.h"
#include "reactor/event_loop.h"
#include "reactor/tcp_connection.h"
#include "web/web_page_query.h"
#include "cache/cache_manager.h"
#include <cstdlib>
#include <vector>


namespace hh {

extern __thread const char * name;
using TcpConnectionPtr = shared_ptr<TcpConnection>;

class WebPageSearcher {
public:
  explicit WebPageSearcher(string sought, TcpConnectionPtr conn,
                           IConfiguration &conf);
  vector<string> DoQuery() const;
  void Process() const {
    auto logger = Logger::GetLogger();
    
    // 在process函数中去进行真正的业务逻辑的处理
    std::cout << "sought in process:" << _sought << '\n';
    logger->info("{} search sought: {}", _tcp_connection->GetPeerAddrString(),
             _sought);

        json result;
        if (CacheManager::GetCacheManager()
                ->GetWebPageCache(atoi(hh::name))
                .Get(_sought, result)) {
          std::cout << "LRUcache hit key" << '\n';
          _tcp_connection->SendInLoop(ProtocolParser::Json2String(result));
        } else {
          std::cout << "LRUcache not hit key" << '\n';
          vector<string> query_ret = DoQuery();
          json cache_push = ProtocolParser::Vector2Json(query_ret);
          CacheManager::GetCacheManager()
              ->GetWebPageCache(atoi(hh::name))
              .Put(_sought, cache_push);
            _tcp_connection->SendInLoop(ProtocolParser::Json2String(cache_push));
        }
    
  }

private:
  string _sought;
  TcpConnectionPtr _tcp_connection;
 
  IConfiguration &_config;
};

} // namespace hh

#endif // WEB_PAGE_SEARCHER_H