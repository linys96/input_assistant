#include "../include/MyLog.hpp"
#include "../include/InputAssistantServer.hpp"
#include "../include/MyTask.hpp"
#include "../include/Myconf.hpp"
#include "../include/TimerThread.hpp"
#include <stdio.h>
#include <string>
#include <json/json.h>

using std::string;

namespace lys
{

InputAssistantServer::InputAssistantServer(Myconf &conf)
:_conf(conf)
,_tcpServer(conf.getConfigMap()["serverIP"],stoi(conf.getConfigMap()["port"]))
,_threadpool(stoi(conf.getConfigMap()["threadpoolCap"]),
             stoi(conf.getConfigMap()["taskQueSize"]))
,_timer(std::bind(&CacheManger::periodicUpdate,CacheManger::getInstance(),
                  _conf.getConfigMap()["cachePath"]),3,6)
{
    _timer.start();
}
 
InputAssistantServer::~InputAssistantServer()
{
    _timer.stop();
}
void InputAssistantServer::onConnection(const TcpConnectionPtr &conn)
{
    /* printf("%s has connectd!\n",conn->toString().c_str()); */
    LogInfo("%s has connectd!\n",conn->toString().c_str());
    conn->send("welcome to server.");
}

void InputAssistantServer::onMessage(const TcpConnectionPtr &conn)
{
    int32_t len = conn->recvInt32();
    
    /* printf("len=%d\n",len); */
    char buff[1024]={0};
    conn->receiveN(buff,len);
    //json解码
    Json::Value root;
    Json::Reader reader;

    if(!reader.parse(buff,root)){
        /* printf("Json parse fail!\n"); */
        LogError("Json parse fail!\n");
        return;
    }
    
    /* printf("buf=%s\n",buff); */
    int i=0;
    string msg = root["queryWord"][i].asString();
    /* printf("msg=%s\n",msg.c_str()); */
    MyTask task(msg,conn); 
    _threadpool.addTask(std::bind(&MyTask::execute,&task));
    /* printf("exit onMessage\n"); */
}

void InputAssistantServer::onClose(const TcpConnectionPtr &conn)
{
    /* printf("%s has closed!\n",conn->toString().c_str()); */
    LogInfo("%s has closed!\n",conn->toString().c_str());
}

void InputAssistantServer::start()
{

    CacheManger* pCacheManger = CacheManger::getInstance();
    pCacheManger->init(stoi(_conf.getConfigMap()["threadpoolCap"]), 
                        _conf.getConfigMap()["cachePath"]);

    _threadpool.start();
 
    using namespace std::placeholders;

    _tcpServer.setConnectionCallback(std::bind(&InputAssistantServer::onConnection,this,_1));
    _tcpServer.setMessageCallback(std::bind(&InputAssistantServer::onMessage,this,_1));
    _tcpServer.setCloseCallback(std::bind(&InputAssistantServer::onClose,this,_1));
     
    _tcpServer.start();

}

}//end of namespace lys
