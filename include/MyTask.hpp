#ifndef __MYTASK_HPP__
#define __MYTASK_HPP__

#include "TcpConnection.hpp"

#include <string>
#include <queue>
#include <vector>
#include <set>

namespace lys
{

struct MyResult
{
    std::string _word; //候选词
    int _iFreq; //词频
    int _iDist; //最小编辑距离
};

class Cache;

class MyCompare
{
public:
    bool operator()(MyResult &lhs, MyResult &rhs)
    {
        if(lhs._iDist!=rhs._iDist){
            //最小编辑距离小的优先
            return lhs._iDist > rhs._iDist;
        }else if(lhs._iFreq!=rhs._iFreq){
            //词频高的优先
            return lhs._iFreq < rhs._iFreq;
        }else{
            //候选词字母表顺序在前的优先
            return lhs._word > rhs._word;
        }
    }
};

class MyTask
{
    const int CANDIDIDATE_NUMBER = 5; //候选词数量

public:
    MyTask(const std::string& queryWord, const TcpConnectionPtr &conn)
    :_queryWord(queryWord)
    ,_conn(conn)
    {
    }

    ~MyTask() {}
    void execute(); //执行查询

private:

    void queryIndexTable(); //查询索引
    void statistic(std::set<int> &iset); //进行计算
    int distance(const std::string &rhs); //计算最小编辑距离

    void response(Cache &cache); //响应客户端的请求
private:
    std::string  _queryWord;
    std::priority_queue<MyResult,std::vector<MyResult>,MyCompare> _resultQue;
    TcpConnectionPtr _conn;
};

}//end of namespace lys

#endif
