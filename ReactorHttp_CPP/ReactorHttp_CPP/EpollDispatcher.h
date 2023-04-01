#pragma once
#include "Dispatcher.h"
#include "Channel.h"
#include "EventLoop.h"
#include <string>
#include <sys/epoll.h>
class EpollDispatcher :
    public Dispatcher
{
public:
    EpollDispatcher(EventLoop* evloop);
    ~EpollDispatcher();

    int add(Channel* channel) override;
    int remove(Channel* channel) override;
    int modify(Channel* channel) override;
    int dispatch(int timeout = 2) override;


private:
    int epollCtl(Channel* channel, int op);

    int m_epfd;
    struct epoll_event* m_events;
    const int m_maxNode = 520;
};

