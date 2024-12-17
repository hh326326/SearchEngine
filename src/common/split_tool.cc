/**
 * @file    :spilt_tool.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-13
 */

#include "common/split_tool.h"
#include "common/file.h"
#include <fstream>
#include <string>

namespace hh {

SplitTool::SplitTool(Logger &logger, IConfiguration &config)
    : _logger(logger), _config(config),
      _seg(_config.GetConfig()["jieba.dict"], _config.GetConfig()["hmm_model"]),
      _jieba(_config.GetConfig()["jieba.dict"],
             _config.GetConfig()["hmm_model"], _config.GetConfig()["user.dict"],
             _config.GetConfig()["idf"], _config.GetConfig()["stop_words"]) {}
/**
 * @brief
 *
 * @param str : string needed to cut
 * @param wordsvec : store result
 * @param flag : true for `Cut with HMM mode`, false for `Cut with mixsegment
 * mode`
 */
void SplitTool::Cut(const string &str, vector<string> &wordsVec, bool flag) {
  // HMM 模式:
  // 该模式一般会考虑上下文信息，使用隐马尔可夫模型来确定最可能的分词结果。对于中文分词，HMM
  // 可以有效处理未登录词（即词典中未出现的词）。
  //  Mixsegment 模式:
  //  可能是一个基于词典和规则的快速分词方式，它会根据字典进行分词，且不考虑太多上下文信息。
  if (flag) {
    _jieba.Cut(str, wordsVec, true); // Cut with HMM mode
  } else {
    _seg.Cut(str, wordsVec); // Cut with mixsegment mode
  }
}

void SplitTool::Cut(const string &str,
                    unordered_map<string, uint64_t> &wordsMap, bool flag) {
  vector<string> tmpWordsVec;
  if (flag) {
    _jieba.Cut(str, tmpWordsVec, true);
  } else {
    _seg.Cut(str, tmpWordsVec);
  }
  // 加载停用词
  set<string> stopwordSet;
  ReadStopWord(stopwordSet, _config.GetConfig()["stop_word_zh"]);
  // 遍历tmpWordVec中的每一个单词(词语)
  for (const auto &elem : tmpWordsVec) {
    // = 0 表示不是停用词
    if (stopwordSet.count(elem) == 0) {
      auto iter = wordsMap.find(elem);
      if (iter != wordsMap.end()) {
        ++iter->second; // 如果当前单词在dictmap中, 词频 ++
      } else {
        wordsMap[elem] = 1; // 否则，词频 = 1
      }
    }
  }
}

/**
 * @brief
 *
 * @param stopword: set<string> & -- 存放停用词
 * @param stopwordpath: const string & -- 停用词路径
 */
void SplitTool::ReadStopWord(set<string> &stopword,
                             const string &stopwordPath) {
  ifstream ifs(stopwordPath);
  if (!ifs) {
    LOG_ERROR("open {} fail.", stopwordPath);
    return;
  }
  string line;
  while (ifs >> line) {
    stopword.insert(line);
  }
  ifs.close();
}

} // namespace hh