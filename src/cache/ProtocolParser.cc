/**
 * @file    :ProtocolParser.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-21
 */

#include "cache/ProtocolParser.h"

namespace hh {
string ProtocolParser::Json2String(json &j) { return j.dump(); }

json ProtocolParser::DoParse(const string &data) { return json::parse(data); }

json ProtocolParser::Vector2Json(vector<string> s) { return json(s); }

void ProtocolParser::Json2Vector(json &j, vector<string> &v) {
  v.clear();
  v = j.get<vector<string>>();
}
} // namespace hh

#if 0
使用方法
json["pi"] = 3.141;
json["happy"] = true;
json["name"] = "whh";
json["answer"]["everything"] = 42;
#endif