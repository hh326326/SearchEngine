/**
 * @file    :web_page_searcher.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-16
 */

#include "web/web_page_searcher.h"
#include "web/web_page_query.h"

#include <utility>

namespace hh {
WebPageSearcher::WebPageSearcher(string sought, TcpConnectionPtr conn,
                                 const PageLibPreprocessor &pre, Logger &logger,
                                 IConfiguration &conf, RedisCache &cache)
    : _sought(std::move(sought)), _tcp_connection(std::move(conn)), _pre(pre),
      _logger(logger), _config(conf), _cache(cache) {}
string WebPageSearcher::DoQuery() const {
  WebPageQuery query(_pre, _logger, _config);
  std::cout << "sought in WebPageSearcher::DoQuery() : " << _sought << '\n';
  return query.DoQuery(_sought);
}

} // namespace hh