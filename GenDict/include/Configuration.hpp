#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

#include <string>
#include <map>

namespace lys
{

class Configuration
{
public:
    Configuration(const std::string &filepath);
    ~Configuration(){}
    
    std::map<std::string,std::string> &getConfigMap();
private:
    std::string _filepath;    
    std::map<std::string,std::string> _configMap;
};

}//end of namespace lys

#endif


