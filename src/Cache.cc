#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Cache.hpp"

using std::cout;
using std::endl;
using std::ofstream;
using std::pair;
using std::ifstream;
using std::string;
using std::istringstream;

namespace lys
{

Cache::Cache(size_t num)
:_maxCacheNum(num)
,_updateCount(0)
,_updateStatus(false)
{

}
Cache::Cache(const Cache& cache)
:_maxCacheNum(100)
,_updateCount(0)
,_updateStatus(true)
,_dataList(cache._dataList)
{
    for (auto it= _dataList.begin(); it != _dataList.end(); ++it) {
        _cacheMap[it->first]=it;
    }
}

Cache &Cache::operator=(const Cache& cache)
{
    if(this != &cache){
        for(auto &node: cache._dataList){
            set(node.first,node.second);
        }
    }
    return *this;
}

void Cache::readFromFile(const std::string &filename)//从文件中读取缓存信息
{
    ifstream ifs(filename);
    if(!ifs.good()) {
        _updateStatus = true;
        cout << "ifs open " << filename << " error!" << endl;
        return;
    }

    string line, key, value;
    while(getline(ifs, line)) {
        istringstream iss(line);
        iss >> key >> value;
        set(key, value);
        key.clear();
        value.clear();
        line.clear();
    }
    ifs.close();

    _updateStatus = true;
}

void Cache::writeToFile(const std::string &filename)//将缓存信息写入文件
{
    ofstream ofs(filename);
    if(!ofs.good()) {
        cout << "ofs open " << filename << " error!" << endl;
        return;
    }

    for(auto &node: _cacheMap){
        ofs << node.second->first << " " << node.second->second << endl;
    }      

    ofs.close();

}

void Cache::update(const Cache &cache)
{
    if( cache._pendingUpdateData.size() > 0) {
        for(auto &node : cache._pendingUpdateData) {
            set(node.first, node.second);
        }
    }
}
void Cache::clearUpdate()
{
    if(_updateCount) {
        _pendingUpdateData.clear();
        _updateCount = 0;
    }
}

string Cache::get(const string &key)
{
    /* showcache(); */
    auto it=_cacheMap.find(key);
    //没找到
    if(it==_cacheMap.end()) {
        return string();
    }
    
    //找到了
    _dataList.splice(_dataList.begin(), _dataList, it->second);
    _cacheMap[key] = _dataList.begin();
    
    /* showcache(); */
    return _cacheMap[key]->second;
}

void Cache::set(const string &key,const string &value)
{
    /* showcache(); */
    auto it= _cacheMap.find(key);
    if(it != _cacheMap.end()) {
        //已存在节点，更新数据
        if(_cacheMap[key]->second != value) {
            _cacheMap[key]->second = value;
        }
        _dataList.splice(_dataList.begin(), _dataList, it->second);
        _cacheMap[key]=_dataList.begin();
    } else {
        //新节点
        if(_dataList.size() == _maxCacheNum) {
            _cacheMap.erase(_dataList.back().first);
            _dataList.pop_back();
        }
        _dataList.push_front(make_pair(key,value));
        _cacheMap[key]=_dataList.begin();

        //为多个cache的更新准备
        if(_updateStatus) {
            ++_updateCount;
            _pendingUpdateData.push_front(make_pair(key, value));
        }
    }
    /* showcache(); */
}

//for test
void Cache::showcache() const
{
    cout << "*********************" << endl;
    for(auto node: _cacheMap){
        cout << node.second->first << " " << node.second->second << endl;
    }
    /* for (auto node: _dataList){ */
    /*     cout << node.first << " " << node.second << endl; */
    /* } */
    cout << "*********************" << endl;
}

}//end of namespace lys
