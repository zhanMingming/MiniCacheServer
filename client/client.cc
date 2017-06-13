#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<iostream>
#include"client.h"


int main(int argc, char **argv)
{
    int sockfd  = socket(AF_INET, SOCK_STREAM, 0);
    CHECK((sockfd == -1), "socket");
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int ret = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(struct sockaddr));
    CHECK(ret == -1, "connect");
    int readCount = 0;
    struct Message sendMessage;
    struct Message recvMessage;
    memset(&sendMessage, 0, sizeof(struct Message));
    memset(&recvMessage, 0, sizeof(struct Message));
    while(fgets(sendMessage.m_text, sizeof(sendMessage.m_text), stdin) != NULL) {
        sendMessage.m_length = strlen(sendMessage.m_text);
        std::cout << "write" << std::endl;
        int ret = writen(sockfd, &sendMessage, (sendMessage.m_length + LENOFINT));
        CHECK(ret == -1, "writen");
        
        readCount = readn(sockfd, &(recvMessage.m_length), LENOFINT);
        CHECK(readCount == -1, "readn"); 
        CHECK(readCount == 0, "connect close");

        if ((readCount = readn(sockfd, &(recvMessage.m_text), recvMessage.m_length)) == -1) {
            ERR_EXIT("read");
        } else if (readCount == 0) {
            ERR_EXIT("connect close");
        }

        recvMessage.m_text[recvMessage.m_length] = '\n';
        std::cout << "sphcache_server:"; fputs(recvMessage.m_text, stdout);
        memset(&sendMessage, 0, sizeof(sendMessage));
        memset(&recvMessage, 0, sizeof(recvMessage));
    }
      
    close(sockfd);
    return 0;
}

