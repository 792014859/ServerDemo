#pragma once
#include "Dispatcher.h"
#include "Channel.h"
#include "EventLoop.h"
#include <string>
#include <sys/select.h>


class SelectDispatcher :
    public Dispatcher
{
public:
    SelectDispatcher(EventLoop* evloop);
    ~SelectDispatcher();
    int add(Channel*& channel) override;
    int remove(Channel*& channel) override;
    int modify(Channel*& channel) override;
    int dispatch(int timeout = 2) override;

private:
    void setFdSet(Channel*& channel);
    void clearFdSet(Channel*& channel);

private:
    fd_set m_readSet;
    fd_set m_writeSet;
    const int m_maxSize = 1024;

};

