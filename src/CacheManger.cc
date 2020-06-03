#include "../include/CacheManger.hpp"
#include "../include/MyLog.hpp"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

namespace lys
{
CacheManger * CacheManger::_pInstance = CacheManger::getInstance();

void CacheManger::init(size_t cacheSize, const string &filename)
{
    _caches.reserve(cacheSize);
    Cache cache;
    cache.readFromFile(filename);

    for(size_t i=0; i != cacheSize; ++i) {
         /* _caches.push_back(Cache()); */
         /* _caches[i].readFromFile(filename); */
        _caches.push_back(cache);
    }
}

Cache &CacheManger::getCache(size_t idx)//获取某个缓存,输入自己的线程id
{
    return _caches[idx];
}

void CacheManger::periodicUpdate(const string &filename)//定时更新所有缓存
{
    size_t cnt = 0;
    for(auto & cache : _caches) {
        cnt += cache.updateCount();
    }
    if(0 == cnt) {
        logInfo("Cache is not need to update!");
        return;
    } else {
        LogInfo("There are %d caches need to update!", cnt);
    }
    //将其他Cache中的新记录添加到0号cache
    for(size_t idx = 1; idx != _caches.size(); ++idx) {
        _caches[0].update(_caches[idx]);
    }
    //将0号cache的内容写入文件
    _caches[0].writeToFile(filename);
    //再将0号cache的新纪录添加到其他Cache之中
    for(size_t idx = 1; idx != _caches.size(); ++idx) {
        _caches[idx].update(_caches[0]);
    }
    //最后清除所有cache中的待更新数据
    for (size_t idx = 0; idx != _caches.size(); ++idx) {
        _caches[idx].clearUpdate();
    }
}

}//end of namespace lys


