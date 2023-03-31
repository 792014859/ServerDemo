#include "SelectDispatcher.h"
#include <stdio.h>
#include <stdlib.h>

SelectDispatcher::SelectDispatcher(EventLoop* evloop) :Dispatcher(evloop)
{
    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
    m_name = "Select";
}

SelectDispatcher::~SelectDispatcher()
{
}

int SelectDispatcher::add(Channel*& channel)
{
    if (channel->getSocket() >= m_maxSize) return -1;
    setFdSet(channel);
    return 0;
}

int SelectDispatcher::remove(Channel*& channel)
{
    clearFdSet(channel);
    channel->destroyCallback(const_cast<void*>(channel->getArg()));
    return 0;
}

int SelectDispatcher::modify(Channel*& channel)
{
    clearFdSet(channel);
    setFdSet(channel);
    return 0;
}

int SelectDispatcher::dispatch(int timeout)
{
    struct timeval val;
    val.tv_sec = timeout;
    val.tv_usec = 0;
    fd_set rdtmp = m_readSet;
    fd_set wrtmp = m_writeSet;
    int count = select(m_maxSize, &rdtmp, &wrtmp, NULL, &val);
    if (count == -1) {
        perror("select");
        exit(0);
    }
    for (int i = 0; i < m_maxSize; ++i) {
        if (FD_ISSET(i, &rdtmp)) {
            m_evloop->eventActive(i, static_cast<int>(FDEvent::ReadEvent));
        }
        if (FD_ISSET(i, &wrtmp)) {
            m_evloop->eventActive(i, static_cast<int>(FDEvent::WriteEvent));
        }
    }
    return 0;
}

void SelectDispatcher::setFdSet(Channel*& channel)
{
    if (channel->getEvent() & static_cast<int>(FDEvent::ReadEvent))
    {
        FD_SET(channel->getSocket(), &m_readSet);
    }
    if (channel->getEvent() & static_cast<int>(FDEvent::WriteEvent))
    {
        FD_SET(channel->getSocket(), &m_writeSet);
    }
}

void SelectDispatcher::clearFdSet(Channel*& channel)
{
    if (channel->getEvent() & static_cast<int>(FDEvent::ReadEvent))
    {
        FD_CLR(channel->getSocket(), &m_readSet);
    }
    if (channel->getEvent() & static_cast<int>(FDEvent::WriteEvent))
    {
        FD_CLR(channel->getSocket(), &m_writeSet);
    }
}
