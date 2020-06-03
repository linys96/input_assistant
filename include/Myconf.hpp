#ifndef __MYCONF_HPP__
#define __MYCONF_HPP__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

namespace lys
{

class Myconf
{
public:
    Myconf(const std::string &filename)
    :_filename(filename)
    {
        std::ifstream ifs(filename);
        if(!ifs.good()){
            std::cout << "open " << filename << "error" << std::endl;
            return;
        }
        std::string key, value, line;
        while(getline(ifs, line)){
            std::istringstream iss(line); 
            iss >> key >> value;
            /* std::cout << key << " " << value << std::endl; */
            _configMap[key]=value;
        }
    }
    ~Myconf() {}
    std::map<std::string,std::string> &getConfigMap()
    {
        return _configMap;
    }

private:
    std::string _filename; 
    std::map<std::string,std::string> _configMap;
};


}//end of namespace lys

#endif
