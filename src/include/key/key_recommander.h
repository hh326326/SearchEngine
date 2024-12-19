/**
 * @file    :key_recommander.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-18
 */

#ifndef __KEY_RECOMMANDER_H__
#define __KEY_RECOMMANDER_H__

#include "key/candidate_result.h"
#include "key/dict_producer.h"
#include "key/dictionary.h"
#include "cache/cache_manager.h"
#include "cache/redis.h"
#include "key/key_query.h"
#include "reactor/event_loop.h"
#include "reactor/tcp_connection.h"
#include "cache/ProtocolParser.h"

#include <iostream>
#include <memory>
#include <queue>
#include <string>


namespace hh {
extern __thread const char *name;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;



class KeyRecommander {
public:
  KeyRecommander(string msg, const TcpConnectionPtr &con,
         KeyQuery &key_query );
  
  //该函数在线程池中执行的
  void Process() const {
    //在process函数中去进行真正的业务逻辑的处理
    std::cout << "message in KeyRecommander:" << _msg << '\n';
        json result;
    if (CacheManager::GetCacheManager()
            ->GetKeyWordCache(atoi(hh::name))
            .Get(_msg, result)) {
      std::cout << "LRUcache hit key" << '\n';
          std::cout << "ProtocolParser::Json2String(result): " << ProtocolParser::Json2String(result) << '\n';
          _con->SendInLoop(ProtocolParser::Json2String(result));
      } else {
        std::cout << "LRUcache not hit key" << '\n';

      std:: cout << "1" << '\n';
      _kq.Query(_msg);
      std:: cout << "2" << '\n';
      json cache_push = ProtocolParser::Vector2Json(_kq.GetString());
      std:: cout << "3" << '\n';
      CacheManager::GetCacheManager()
          ->GetKeyWordCache(atoi(hh::name))
          .Put(_msg, cache_push);
      std:: cout << "4" << '\n';
      string send_ms = ProtocolParser::Json2String(cache_push);
      std::cout << "send_ms: " << send_ms << '\n';
      _con->SendInLoop(send_ms);
      }
  }


private:

  string _msg;
  TcpConnectionPtr _con;
  KeyQuery &_kq;

};
} // namespace hh

#endif // KEY_REMOCCANDER_H