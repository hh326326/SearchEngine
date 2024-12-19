/**
 * @file    :key_recommander.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-15
 */

#include "key/key_recommander.h"
#include "key/candidate_result.h"
#include "key/dictionary.h"
#include "key/key_query.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace hh {
using std::string;

  KeyRecommander::KeyRecommander(string msg, const TcpConnectionPtr &con,
         KeyQuery &key_query )
      : _msg(std::move(msg)), _con(std::move(con)), _kq(key_query)
        {}

} // namespace hh
