#include "TcpServer.h"
#include <arpa/inet.h>
#include "TcpConnection.h"
#include <stdio.h>
#include <stdlib.h>
#include "Log.h"
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int TcpServer::acceptConnection(void* arg)
{
    TcpServer* server = static_cast<TcpServer*>(arg);
    // 和客户端建立连接
    int cfd = accept(server->m_lfd, NULL, NULL);
    if (cfd == -1 && errno == EMFILE) {
        close(server->freeid);
        server->freeid = accept(server->m_lfd, NULL, NULL);
        close(server->freeid);
        server->freeid = open("/dev/null", O_RDONLY | O_CLOEXEC);
        //perror("accept");
    }
    if (cfd > 0) {
        // 从线程池中取出一个子线程的反应堆实例, 去处理这个cfd
        EventLoop* evLoop = server->m_threadPool->takeWorkerEventLoop();
        // 将cfd放到 TcpConnection中处理
        new TcpConnection(cfd, evLoop);
    }
    return 0;
}

TcpServer::TcpServer(const unsigned short port, const int threadNum, const int dispatcherType) 
    : m_port(port), m_threadNum(threadNum), m_dispatcherType(dispatcherType)
{
    //生成主反应堆实例
    m_mainLoop = new EventLoop(m_dispatcherType);
    //生成线程池实例
    m_threadPool = new ThreadPool(m_mainLoop, m_threadNum);
    freeid = open("/dev/null", O_RDONLY | O_CLOEXEC);
    //设置监听
    setListen();
}

TcpServer::~TcpServer()
{
    if (m_mainLoop != nullptr) {
        delete m_mainLoop;
        m_mainLoop = nullptr;
    }
    if (m_threadPool != nullptr) {
        delete m_threadPool;
        m_threadPool = nullptr;
    }
}

void TcpServer::setListen()
{
    // 1. 创建监听的fd
    m_lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_lfd == -1)
    {
        perror("socket");
        return;
    }
    // 2. 设置端口复用
    int opt = 1;
    int ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
    if (ret == -1)
    {
        perror("setsockopt");
        return;
    }
    // 3. 绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    ret = bind(m_lfd, (struct sockaddr*)&addr, sizeof addr);
    if (ret == -1)
    {
        perror("bind");
        return;
    }
    // 4. 设置监听
    ret = listen(m_lfd, 128);
    if (ret == -1)
    {
        perror("listen");
        return;
    }
}

void TcpServer::run()
{
    Debug("服务器程序已经启动了...");
    // 启动线程池
    m_threadPool->run(m_dispatcherType);    
    // 添加检测的任务：初始化一个channel实例，将其加入到主反应堆的任务队列中
    Channel* channel = new Channel(m_lfd, FDEvent::ReadEvent, acceptConnection, nullptr, nullptr, this);
    m_mainLoop->addTask(channel, ElemType::ADD);
    // 启动反应堆模型
    m_mainLoop->run();
}
