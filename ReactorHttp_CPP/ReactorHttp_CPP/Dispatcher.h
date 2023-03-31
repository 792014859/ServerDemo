#pragma once
#include "Channel.h"
#include "EventLoop.h"
#include <string>

class EventLoop;
class Dispatcher
{
public:
	Dispatcher(EventLoop* evloop) : m_evloop(evloop) {};
	virtual ~Dispatcher() = default;

	//添加
	virtual int add(Channel*& channel) = 0;
	//删除
	virtual int remove(Channel*& channel) = 0;
	//修改
	virtual int modify(Channel*& channel) = 0;
	//事件检测
	virtual int dispatch(int timeout = 2) = 0;//单位:s

	//inline void setChannel(Channel* channel) { m_channel = channel; }

protected:
	std::string m_name = std::string();
	//Channel* m_channel;
	EventLoop* m_evloop;

private:

};

