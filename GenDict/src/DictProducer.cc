#include "../include/DictProducer.hpp"
#include "../include/SplitToolJieba.hpp"
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <set>

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::pair;
using std::unordered_set;
using std::map;
using std::set;

//1. 求取一个字符占据的字节数
size_t nBytesCode(const char ch);

//2. 求取一个字符串的字符长度
std::size_t length(const std::string &str);

//3. 中英文通用的最小编辑距离算法
int editDistance(const std::string & lhs, const std::string &rhs);

namespace lys
{
DictProducer::DictProducer(const std::string &dir)
:_dir(dir)
,_splitTool(nullptr)
{
}

DictProducer::DictProducer(const std::string &dir, SplitTool *splitTool)
:_dir(dir)
,_splitTool(splitTool)
{
}

void DictProducer::readStopWords(const char *filepath)
{
    ifstream ifs(filepath);
    if(!ifs.good()){
        cout << "ifs open " << filepath << "error" << endl;
        return;
    }
    string line;
    while(getline(ifs,line)) {
        _stopWords.insert(line);
        line.clear();
    }
    ifs.close();
    
}

void DictProducer::buildDict()
{
    getFiles();    

    //读取语料库
    for(auto &file: _files) {
        ifstream is(file);
        char ch;
        bool emptyFlag = true;
        string word;
        while(is.get(ch)) {
            if(isspace(ch) || ispunct(ch)) {
                if(!emptyFlag) {

                    if(_stopWords.find(word) == _stopWords.end()) {
                       pushDict(word); 
                    }
                    word.clear();
                    emptyFlag = true;
                }
            }else if(isalpha(ch)){
                ch = tolower(ch);
                word.push_back(ch);
                emptyFlag = false;
            }
        }
        is.close();

    }
}

void DictProducer::buildCnDict()
{
    getFiles();
    
    //读取语料库
    for (auto &filename: _files){

        ifstream ifs(filename,std::ios::ate); 
        size_t lenth = ifs.tellg();
        char *buff = new char[lenth + 1](); 
        ifs.seekg(0);
        ifs.read(buff,lenth);
        string content (buff);
        vector<string> tmp = _splitTool->cut(content);
        
        for(auto &word: tmp){
            if(_stopWords.find(word)==_stopWords.end()){
                if(nBytesCode(word[0])==3){
                    pushDict(word);
                }
            }
        }
        content.assign("");
        tmp.clear();
        ifs.close();
    } 
}

void DictProducer::buildIndex()
{
    int i=0;
    for(auto & elem: _dict){

        string tmp = elem.first;
        size_t lenth = length(tmp);

        for (size_t idx = 0, n = 0; n != lenth ; ++idx, ++n){
            size_t nBytes = nBytesCode(tmp[idx]);
            string subtmp = tmp.substr(idx, nBytes);  

            _index[subtmp].insert(i);
            idx += (nBytes - 1);
        }

        ++i;
    }  
}


void DictProducer::storeDict(const char* filepath)
{
    ofstream ofs(filepath);
    if(!ofs.good()){
        cout << "ofs open " << filepath << "error!" << endl;
        return;
    }
    for (auto &elem : _dict){
        ofs << elem.first << " " << elem.second << endl;
    }

    ofs.close();
    cout << "store dict file success!" << endl;
}

void DictProducer::storeIndex(const char* filepath)
{
    ofstream ofs(filepath);

    if(!ofs.good()){
        cout << "ofs open " << filepath << "error!" << endl;
        return;
    }

    for (auto &elem : _index){
        ofs << elem.first ;
        for (auto &idx: elem.second){
            ofs << " " << idx ;
        }
        ofs << endl;
    }

    ofs.close();

    cout << "store index file success!" << endl;
}

void DictProducer::showFiles() const
{
    for(auto &s: _files){
        cout << s << endl;
    } 
}

void DictProducer::showDict() const
{
    for(auto &word : _dict){
        cout << word.first << " " << word.second << endl;
    }    
}

void DictProducer::getFiles()
{

    DIR *pdir = ::opendir(_dir.c_str());   
    if(NULL == pdir){
        perror("opendir");
        return;
    }
    struct dirent *pDirInfo;
    while( (pDirInfo=readdir(pdir)) != NULL ){
        if(0 == strcmp(pDirInfo->d_name, ".")||
           0 == strcmp(pDirInfo->d_name, "..")){
            continue;
        }
        char tmp[512]={0};
        sprintf(tmp,"%s/%s",_dir.c_str(),pDirInfo->d_name);
        _files.push_back(string(tmp));
    }

}

inline
void DictProducer::pushDict(const std::string &word)
{
    ++_dict[word];
}


}//end of namespace lys
