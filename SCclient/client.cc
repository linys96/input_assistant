#include "../include/SocketIO.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <json/json.h>
#include <string>

using std::string;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void do_service(int sockfd);

int main(int argc, const char *argv[])
{
    if(argc != 3) {
        printf("./client.exe IP PORT\n");
        return -1;
    }

    int peerfd = socket(PF_INET, SOCK_STREAM, 0);
    if(peerfd == -1)
        ERR_EXIT("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    socklen_t len = sizeof addr;
    if(connect(peerfd, (struct sockaddr*)&addr, len) == -1)
        ERR_EXIT("Connect");

	char buf[1024];
	memset(buf, 0, sizeof(buf));
	read(peerfd, buf, sizeof(buf));
	printf("%s\n", buf);

    do_service(peerfd);
    return 0;
}



void do_service(int sockfd)
{
    char recvbuf[1024] = {0};
    char sendbuf[1024] = {0};
    lys::SocketIO sockIO(sockfd); 

    while(1)
    {
        Json::Value root;
        Json::FastWriter fwriter;
        Json::Reader reader;
        /* queryWord.clear(); */
        /* root.clear(); */
        printf("--------------------------\n");
        printf("Input your query:\n");
        fgets(sendbuf, sizeof sendbuf, stdin);
        sendbuf[strlen(sendbuf)-1]='\0';
        root["queryWord"].append(sendbuf);
        string sendstr=fwriter.write(root);
        /* printf("%s\n",sendstr.c_str()); */
        sockIO.writeInt32(sendstr.size());
        sockIO.writen(sendstr.c_str(),sendstr.size());
        /* write(sockfd, sendbuf, strlen(sendbuf)); */
        
        
        //read
        /* int nread = read(sockfd, recvbuf, sizeof recvbuf); */
        int32_t nread;
        sockIO.readInt32(nread);
        if(nread == -1)
        {
            if(errno == EINTR)
                continue;
            ERR_EXIT("read");
        }
        else if(nread == 0) //代表链接断开
        {
            printf("server close!\n");
            close(sockfd);
            exit(EXIT_SUCCESS);
        }
        sockIO.readn(recvbuf,nread); 
        printf("%s\n",recvbuf);
        if(!reader.parse(recvbuf,root)){
            printf("Json parse fail!\n");
            return;
        }
        int sz=root["words"].size();
        int i=0;
        printf("--------------------------\n");
        printf("For your query of %s, we find %d similar words:\n",root["queryWord"][i].asString().c_str(),sz);
        for(i=0; i!=sz; ++i){
            printf("%s\n",root["words"][i].asString().c_str());
        }
        memset(recvbuf, 0, sizeof(recvbuf));
        memset(sendbuf, 0, sizeof(sendbuf));
    }
}
