#ifndef __DICTPRODUCER_HPP__
#define __DICTPRODUCER_HPP__
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>

namespace lys
{

class SplitTool;

class DictProducer
{
public:
    DictProducer(const std::string &dir);
    DictProducer(const std::string &dir, SplitTool *splitTool);
    ~DictProducer() {}

    void buildDict(); //创建英文字典
    void buildCnDict(); //创建中文字典
    void buildIndex(); //创建索引
    void readStopWords(const char* filepath); //读取停用词
    void storeDict(const char* filepath); //将词典写入文件
    void storeIndex(const char* filepath); //将索引写入文件
    void showFiles() const; //查看文件路径，测试用
    void sholysict() const; //查看词典，测试用
private:
    void getFiles(); //获取文件的绝对路径
    void pushDict(const std::string &word); //存储某个单词
private:
    std::string _dir; //语料库文件夹路径
    std::vector<std::string> _files; //语料库文件的绝对路径集合
    std::unordered_set<std::string> _stopWords;  //停用词
    std::map<std::string,int> _dict; //词典
    std::unordered_map<std::string,std::set<int>> _index; //索引

    SplitTool *_splitTool; //分词工具
};

}//end of namespace lys

#endif

