#ifndef __SPLITTOOL_HPP__
#define __SPLITTOOL_HPP__

#include <vector>
#include <string>

namespace lys
{

class SplitTool
{
public:
    SplitTool() {}
    virtual ~SplitTool() {}
    
    virtual std::vector<std::string> cut(const std::string& sentence)=0;
};

class Configuration;

class SplitToolCppJieba
:public SplitTool
{
public:
    SplitToolCppJieba(Configuration &conf);
    virtual ~SplitToolCppJieba()
    {
    }

    virtual std::vector<std::string> cut(const std::string &sentence) ; 
private:
    Configuration &_conf;
};

}//end of namespace lys



#endif
