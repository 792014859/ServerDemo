#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include "TcpServer.h"

int main(int argc, char* argv[])
{
#if 0
    if (argc < 3)
    {
        printf("./a.out port path\n");
        return -1;
    }
    unsigned short port = atoi(argv[1]);
    // 切换服务器的工作路径
    chdir(argv[2]);
#else
    unsigned short port = 10000;
    chdir("/home/zxd/luffy");
#endif
    // 启动服务器
    int threadNums = std::thread::hardware_concurrency();
    //std::cout << "服务器线程数：" << threadNums << std::endl;
    int dispatcherType = 3;
    TcpServer* server = new TcpServer(port, threadNums, dispatcherType);
    server->run();

    return 0;
}