#include "../include/Mydict.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::set;
using std::pair;
using std::unordered_map;
using std::ifstream;
using std::istringstream;

namespace lys
{
lys::Mydict* lys::Mydict::_pInstance=createInstance();

Mydict* Mydict::createInstance()   
{
    if(nullptr == _pInstance){
        _pInstance = new Mydict();
    }
    return _pInstance;
}

void Mydict::destroy()
{
    if(_pInstance){
        delete _pInstance;
        _pInstance = nullptr;
    }
}

void Mydict::init(const std::string &dictEnPath, const std::string &idxEnPath,
              const std::string &dictCnPath, const std::string &idxCnPath)
{
    ifstream Enifs(dictEnPath), Cnifs(dictCnPath), EnIdxifs(idxEnPath), CnIdxifs(idxCnPath); 
    if(!(Enifs.good() && Cnifs.good() && EnIdxifs.good() && CnIdxifs.good())){
        cout << "Mydict open file error!" << endl;
        return;
    }
    string line, word; 
    int freq,idx;
    while(getline(Enifs,line)){
        istringstream iss(line); 
        iss >>  word >> freq;
        /* cout << idx << " " << word << " " << freq << endl; */
        _dict.push_back(make_pair(word,freq));
    }

    int enWordSize=_dict.size();

    while(getline(Cnifs,line)){
        istringstream iss(line); 
        iss >>  word >> freq;
        _dict.push_back(make_pair(word,freq));
    }

    while(getline(EnIdxifs,line)){
        /* cout << line << endl; */
        istringstream iss(line); 
        iss >> word;
        /* cout << word << ":" ; */
        while(iss>>idx){
            /* cout << idx << " "; */
            _indexTable[word].insert(idx);
        } 
        /* cout << endl; */
    }
    while(getline(CnIdxifs,line)){
        istringstream iss(line); 
        iss >> word;
        while(iss>>idx){
            _indexTable[word].insert(idx+enWordSize);
        } 
    }
}

vector<pair<string,int>> &Mydict::getDict()
{
    return _dict;
}

unordered_map<string,set<int>> &Mydict::getIndexTable()
{
    return _indexTable;
}

}//end of namespace lys

