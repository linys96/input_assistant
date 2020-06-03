#ifndef __CACHE_HPP__
#define __CACHE_HPP__

#include <unordered_map>
#include <string>
#include <list>

namespace lys
{

class Cache//缓存类
{
public:
    using CacheNode = std::pair<std::string,std::string>;
    using iterator = std::list<CacheNode>::iterator;

    Cache(size_t num = 100);

    Cache(const Cache& cache);
    Cache &operator=(const Cache& cache);

    void readFromFile(const std::string &filename);//从文件中读取缓存信息
    void writeToFile(const std::string &filename);//将缓存信息写入文件

    void update(const Cache &);//更新缓存信息

    size_t updateCount() const 
    {
        return  _updateCount;
    }
    void clearUpdate();

    std::string get(const std::string &key);
    void set(const std::string &key, const std::string &val);

    void showcache() const;//测试用

private:

    size_t _maxCacheNum; //能容纳的总记录数
    size_t _updateCount; //待更新的记录数
    bool _updateStatus; //更新标志位

    std::unordered_map<std::string,iterator> _cacheMap;//缓存
    std::list<CacheNode> _dataList;
    std::list<CacheNode> _pendingUpdateData; //待更新的数据

};

}
#endif
