#include "../include/SplitToolJieba.hpp"
#include "../include/cppjieba/Jieba.hpp"
#include "../include/Configuration.hpp"

#include <iostream>
using std::vector;
using std::string;
using std::cout;
using std::endl;


namespace lys
{

SplitToolCppJieba::SplitToolCppJieba(Configuration &conf)
:_conf(conf)
{
}

std::vector<std::string> SplitToolCppJieba::cut(const std::string &sentence) 
{

    const char* const DICT_PATH = _conf.getConfigMap()["DICT_PATH"].c_str();
    const char* const HMM_PATH = _conf.getConfigMap()["HMM_PATH"].c_str();
    const char* const USER_DICT_PATH = _conf.getConfigMap()["USER_DICT_PATH"].c_str();
    const char* const IDF_PATH = _conf.getConfigMap()["IDF_PATH"].c_str();
    const char* const STOP_WORD_PATH = _conf.getConfigMap()["STOP_WORD_PATH"].c_str();
    cppjieba::Jieba jieba(DICT_PATH,
                          HMM_PATH,
                          USER_DICT_PATH,
                          IDF_PATH,
                          STOP_WORD_PATH);

    vector<string> words;
    jieba.CutForSearch(sentence,words);

    return  words;
}

}//end of namespace lys
