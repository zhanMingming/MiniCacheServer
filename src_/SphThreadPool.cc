#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "common.h"
#include "SphMyThread.h"
#include "SphThreadPool.h"

void strTrimCrlf(char *str)
{
    char *p = &str[strlen(str)-1];
    while (*p == '\r' || *p == '\n') {
        *p-- = '\0';
    }
}

void str_split(const char *str, char *cmd, char *rest, char c)
{
    const char *p = strchr(str, c); 
    if (p == NULL) {
        strcpy(cmd, str);
    } else {
        strncpy(cmd, str, p - str);
        strcpy(rest, p + 1);
    }
}

void str_upper(char *str)
{
    while (*str) {
        *str = toupper(*str);
        ++str;
    }
}



size_t readn(int fd, void *buf, size_t count)
{
    std::cout << "start to readi :" << count << std::endl;
    size_t nleft = count;
    ssize_t nread;
    char *bufp = (char*)buf;
    while (nleft > 0) {
        if (( nread = read(fd, bufp, nleft)) < 0) {
            if (errno == EINTR || errno == EAGAIN ) {
                continue;
                return -1;
            } 
        } else if (nread == 0) {
            return nread;
        }
        sleep(1);
        std::cout << " have read it:" << nread << std::endl;
        bufp += nread;
        nleft -= nread;
    }
    return count;
}

size_t writen(int fd, void* buf, size_t count)
{
    std::cout << "start to write" << std::endl;
    size_t nleft = count;
    ssize_t nwritten;
    char *bufp = (char*)buf;
    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) < 0) {
            if (errno == EINTR) {
                continue;
                return -1;
            }
        } else if (nwritten == 0) {
            return nwritten;
        }
        std::cout << "have write it :" << nwritten << std::endl;
        bufp += nwritten;
        nleft -= nwritten;
    }
    return count;
}

namespace zhanmm {

struct Message
{
    int m_length;
    char m_text[1024 * 20];
};

SphThreadPool::SphThreadPool(Session *sess, SphEpoller &epoll)
:m_sess(sess),
cache(sess),
m_queFd(INT_MAX),
m_listNode(INT_MAX),
m_epoll(epoll),
m_isExit(false)
{}

SphThreadPool::~SphThreadPool()
{
    stop();
}

void SphThreadPool::add(int fd)
{
    printf("I'm the  main thread, put fd into  queue, My id = %lld\n", pthread_self());
    m_queFd.push(fd);
}

void SphThreadPool::start()
{
    SphThread *pthread = new SphMyThread(*this, ThreadType::SyncThread);
    pthread->start();
    m_vecThreads.push_back(pthread);
    int nums = (m_sess->threadNums - 1) < 1? 1 : (m_sess->threadNums -1);
  
    for (int index = 0; index != nums; ++index) 
    {
        pthread = new SphMyThread(*this, ThreadType::WorkerThread);
        pthread->start();
        m_vecThreads.push_back(pthread);
    }
}

void SphThreadPool::stop()
{
    if (!m_isExit) {
        m_isExit = true;
        std::vector<SphThread*>::iterator iter;
        for (iter = m_vecThreads.begin(); iter !=m_vecThreads.end();++iter)
        {
            (*iter)->join();
        }
    }
}

void SphThreadPool::doWorker()
{

    printf("I'm a Worker thread, id = %lld\n", pthread_self());
    while(1) {        
        int fd = m_queFd.pop();
        printf("I'm a Worker thread, id = %lld", pthread_self());
        printf(" And  i get fd is  %d\n", fd);
        while (1) {
            struct  Message recvMessage;
            struct  Message sendMessage;
            memset(&recvMessage, 0, sizeof(struct Message));
            memset(&sendMessage, 0, sizeof(struct Message));
            int readCount = 0;
            std::cout << "read" << std::endl;
            readCount = readn(fd, &(recvMessage.m_length), LENOFINT);
            CHECK(readCount == -1, "readn");
            if (readCount == 0) {
                perror("connect close");
                close(fd);
                break;
            }
            readCount = readn(fd, &(recvMessage.m_text), recvMessage.m_length);
            CHECK(readCount == -1, "readn");
            if (readCount == 0) {
                perror("connect close");
                close(fd);
                break;
            }

            strTrimCrlf(recvMessage.m_text);
            char cmd[1024] = {0};
            char key[1024] = {0};
            char rest[1024 *20] = {0};
            str_split(recvMessage.m_text, cmd, rest, ' ');
            str_upper(cmd);
            if (strlen(rest) == 0) {
                sendMessage.m_length = strlen(NO_COMMAND);
                strncpy(sendMessage.m_text, NO_COMMAND, sendMessage.m_length);
                int ret = writen(fd, &sendMessage, sendMessage.m_length + LENOFINT);
                CHECK(ret == -1, "writen");
                continue;
            }
            if (strncmp(cmd, "GET", 3) == 0) {
                std::string ret = cache.get(rest);
                if (!ret.empty())  {
                    sendMessage.m_length = ret.size();
                    strncpy(sendMessage.m_text, ret.c_str(), sendMessage.m_length);
                    int ret = writen(fd, &sendMessage, sendMessage.m_length + LENOFINT);
                    CHECK(ret == -1, "writen");
                } else {
                    sendMessage.m_length = strlen(NO_THISKEY);
                    strncpy(sendMessage.m_text, NO_THISKEY, sendMessage.m_length);
                    int ret = writen(fd, &sendMessage, sendMessage.m_length + LENOFINT);
                    CHECK(ret == -1, "writen");
                }
            } else if (strncmp(cmd, "SET", 3) == 0) {
                char value[1024 *20] = {0};
                str_split(rest, key, value, ' ');
                if (strlen(value) == 0) {
                    sendMessage.m_length = strlen(NO_THISKEY);
                    strncpy(sendMessage.m_text, NO_THISKEY, sendMessage.m_length);
                    int ret = writen(fd, &sendMessage, sendMessage.m_length + LENOFINT);
                    CHECK(ret == -1, "writen");
                } else {
                    std::string k(key), v(value);
                    cache.set(k, v);
                    std::cout << "key" << k << std::endl;
                    std::cout << "value" << v << std::endl;
                    sendMessage.m_length = strlen(SET_SUCESS);
                    strncpy(sendMessage.m_text, SET_SUCESS, sendMessage.m_length);
                    int ret = writen(fd, &sendMessage, sendMessage.m_length + LENOFINT);
                    CHECK(ret == -1, "writen");
                }

            } else  {
                sendMessage.m_length = strlen(NO_THISKEY);
                strncpy(sendMessage.m_text, NO_THISKEY, sendMessage.m_length);
                int ret = writen(fd, &sendMessage, sendMessage.m_length + LENOFINT);
                CHECK(ret == -1, "writen");
            }
/*
            struct epoll_event ev;
            ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
            m_epoll.add(fd, ev);
            break;            
*/
        }
    }
}

void SphThreadPool::doSync()
{
    while(1) {     
        printf("I'm a doSync thread, id = %lld\n", pthread_self());
        std::cout << m_listNode.pop() << std::endl;
    }
}

} // namespce zhanmm


     
 
