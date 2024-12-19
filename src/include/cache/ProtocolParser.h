/**
 * @file    :ProtocolParser.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-21
 */

#ifndef __PROTOCOLPARSER_H__
#define __PROTOCOLPARSER_H__

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include <string>
#include <vector>

namespace hh {

enum
{
KeyRecommand,
WebPageSearch
};

using nlohmann::json;
using std::string;
using std::vector;

class ProtocolParser {
public:
  static string Json2String(json &j);
  static json DoParse(const string &data);
  static json Vector2Json(vector<string> s);
  static void Json2Vector(json &j, vector<string> &v);
};

} // namespace hh

#endif // PROTOCOLPARSER_H