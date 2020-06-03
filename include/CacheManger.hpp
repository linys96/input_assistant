#ifndef __CACHEMANGER_HPP__
#define __CACHEMANGER_HPP__

#include "Cache.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

namespace lys
{
class CacheManger
{
public:

    static CacheManger* getInstance()
    {
        if(nullptr == _pInstance) {
            _pInstance = new CacheManger();
        }
        return _pInstance;
    }

    static void destroy()
    {
        if(_pInstance) {
            delete _pInstance;
            _pInstance = nullptr;
        }
    }

    void init(size_t t, const std::string &filename);//从磁盘文件中读取缓存信息
    Cache &getCache(size_t idx);//获取某个缓存
    void periodicUpdate(const std::string &filename);//定时更新所有缓存

private:
    CacheManger() {}
    ~CacheManger() {}
private:
    static CacheManger * _pInstance;
    std::vector<Cache> _caches;
};

}//end of namespace lys

#endif
