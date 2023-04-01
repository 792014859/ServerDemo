#pragma once
#include "Dispatcher.h"
#include "Channel.h"
#include "EventLoop.h"
#include <string>
#include <poll.h>
class PollDispatcher :
    public Dispatcher
{
public:
    PollDispatcher(EventLoop* evloop);
    ~PollDispatcher();
    int add(Channel* channel) override;
    int remove(Channel* channel) override;
    int modify(Channel* channel) override;
    int dispatch(int timeout = 2) override;

private:
    int m_maxfd;
    struct pollfd* m_fds;
    const int m_maxNode = 1024;
};

