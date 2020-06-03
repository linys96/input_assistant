#define _LOG4CPP_

#include "../include/MyLog.hpp"
#include "../include/Myconf.hpp"
#include "../include/InputAssistantServer.hpp"
#include "../include/Mydict.hpp"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main(int argc,char *argv[])
{
    lys::Myconf conf("../conf/server.conf");
    
    lys::Mydict *pInstance = lys::Mydict::createInstance();    
    pInstance->init(conf.getConfigMap()["enDictPath"],conf.getConfigMap()["enIndexTablePath"],
                    conf.getConfigMap()["cnDictPath"],conf.getConfigMap()["cnIndexTablePath"]);
    lys::InputAssistantServer iaServer(conf);
    
    iaServer.start();

    return 0;
}

