#ifndef __MYDICT_HPP__
#define __MYDICT_HPP__

#include <vector>
#include <unordered_map>
#include <string>
#include <set>

//这个类应该是要放在第一阶段的
//老师给的文档坑爹
//整合的时候要看看第一阶段怎么写的

//单例模式
namespace lys
{

class Mydict
{
public:
    static Mydict* createInstance();    
    void destroy();
    void init(const std::string &dictEnPath,const std::string &idxEnPath,
              const std::string &dictCnPath,const std::string &idxCnPath);//通过中英文词典文件路径初始化词典
    std::vector<std::pair<std::string,int>> &getDict(); //获取词典
    std::unordered_map<std::string,std::set<int>> &getIndexTable(); //获取索引表
    /* std::vector<std::pair<std::string,int>> &getCnDict(); //获取中文词典 */
    /* std::unordered_map<std::string,std::set<int>> &getCnIndexTable(); //获取中文索引表 */
    /* std::vector<std::pair<std::string,int>> &getEnDict(); //获取英文词典 */
    /* std::unordered_map<std::string,std::set<int>> &getEnIndexTable(); //获取英文索引表 */
private:
    Mydict() {}
    ~Mydict() {}

private:
    std::vector<std::pair<std::string,int>> _dict; //中英文词典
    std::unordered_map<std::string,std::set<int>> _indexTable; //中英文索引表
    /* std::vector<std::pair<std::string,int>> _Cndict; //中文词典 */
    /* std::vector<std::pair<std::string,int>> _Endict; //英文词典 */
    /* std::unordered_map<std::string,std::set<int>> _CnindexTable; //中文索引表 */
    /* std::unordered_map<std::string,std::set<int>> _EnindexTable; //英文索引表 */
    static Mydict *_pInstance;
};


}//end of namespace lys

#endif
