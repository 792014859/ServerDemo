#pragma once
#include "EventLoop.h"
#include "ThreadPool.h"

class TcpServer
{
public:
    TcpServer(const unsigned short port, const int threadNum, const int dispatcherType = 2);
    ~TcpServer();
    // 初始化监听
    void setListen();
    // 启动服务器
    void run();
    static int acceptConnection(void* arg);

private:
    int m_threadNum;
    EventLoop* m_mainLoop;
    ThreadPool* m_threadPool;
    int m_lfd;
    unsigned short m_port;
    int m_dispatcherType;
    int freeid;
};

