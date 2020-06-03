#include "../include/Configuration.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using std::map;
using std::string;
using std::ifstream;
using std::istringstream;
using std::cout;
using std::endl;

namespace lys
{

Configuration::Configuration(const std::string &filepath)
:_filepath(filepath)
{
    ifstream ifs(filepath);
    if(!ifs.good()){
       cout << "Configuration file open error!" << endl;
    }

    string line, key ,value;

    while(getline(ifs,line)){
        istringstream iss(line);
        iss >> key >> value;
        _configMap[key] = value;
    }
    ifs.close();
}

std::map<std::string,std::string> &Configuration::getConfigMap()
{
    return _configMap;
}


}//end of namespace lys
