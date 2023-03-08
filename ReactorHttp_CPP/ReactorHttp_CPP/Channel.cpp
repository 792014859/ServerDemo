#include "Channel.h"
using namespace std;
Channel::Channel(int _fd, FDEvent _events, handleFunc _readFunc, handleFunc _writeFunc, handleFunc _destroyFunc, void* _arg)
{
	m_fd = _fd;
	m_events = static_cast<int>(_events);
	m_arg = _arg;
	readCallback = _readFunc;
	writeCallback = _writeFunc;
	destroyCallback = _destroyFunc;
}

void Channel::writeEventEnable(bool flag)
{
	if (flag) {
		m_events |= static_cast<int>(FDEvent::WriteEvent);
	}
	else {
		m_events = m_events & ~(static_cast<int>(FDEvent::WriteEvent));
	}
}

bool Channel::isWriteEventEnable()
{
	return m_events & static_cast<int>(FDEvent::WriteEvent);
}
