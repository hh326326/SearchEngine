/**
 * @file    :settion.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-18
 */

#ifndef __SETTION_MANAGER_H__
#define __SETTION_MANAGER_H__

#include "common/configuration.h"
#include "key/dictionary.h"
#include "key/key_query.h"
#include "web/page_lib_preprocessor.h"

namespace hh {

struct Setting {
  Setting(Dictionary &dict, KeyQuery &kq, PageLib &pl )
      : _dict(dict), _key_query(kq), _plpage_lib(pl)
         {
  }

  PageLib &_plpage_lib;
  Dictionary &_dict;
  KeyQuery &_key_query;
};

class SettingManager {
public:
  SettingManager(Configuration &conf)
      :  config(conf), dict(config), keyQue(dict),
        pl(config)  {}

  Setting createSetting() { return Setting(dict, keyQue, pl ); }

private:
  Configuration &config;
  Dictionary dict;
  KeyQuery keyQue;
  PageLib pl;

};

} // namespace hh

#endif // SETTION_MANAGER_H