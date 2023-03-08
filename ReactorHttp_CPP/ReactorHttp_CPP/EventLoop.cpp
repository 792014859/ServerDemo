#include "EventLoop.h"
#include <assert.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EpollDispatcher.h"

using namespace std;

EventLoop::EventLoop() :EventLoop(string())
{
}

EventLoop::EventLoop(const std::string threadName)
{
	m_isQuit = true;
	m_threadID = this_thread::get_id();
	m_threadName = threadName == string() ? "MainThread" : threadName;
	m_dispatcher = new EpollDispatcher(this);//可以通过工厂模式进行修改
	m_channelMap.clear();
	int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, m_socketPair);
	if (ret == -1) {
		perror("socketpair");
		exit(0);
	}
#if 0
	Channel* channel = new Channel(m_socketPair[1], FDEvent::ReadEvent, readLocalMessage, nullptr, nullptr, this);
#else 
	auto obj = bind(&EventLoop::readMessage, this);
	Channel* channel = new Channel(m_socketPair[1], FDEvent::ReadEvent, obj, nullptr, nullptr, this);
#endif
	//channel添加到任务队列
	addTask(channel, ElemType::ADD);
}

EventLoop::~EventLoop()
{
}

int EventLoop::run()
{
	m_isQuit = false;
	//比较线程ID是否正常
	if (m_threadID != this_thread::get_id()) {
		return -1;
	}
	//循环进行事件处理
	while (!m_isQuit) {
		m_dispatcher->dispatch();
		processTaskQ();
	}
	return 0;
}

int EventLoop::eventActive(int fd, int event)
{
	if (fd < 0) return -1;
	Channel* channel = m_channelMap[fd];
	assert(channel->getSocket() == fd);
	if (event & static_cast<int>(FDEvent::ReadEvent) && channel->readCallback) {
		channel->readCallback(const_cast<void*>(channel->getArg()));
	}
	if (event & static_cast<int>(FDEvent::WriteEvent) && channel->writeCallback) {
		channel->writeCallback(const_cast<void*>(channel->getArg()));
	}
	return 0;
}

int EventLoop::addTask(Channel* channel, ElemType type)
{
	m_mutex.lock();
	ChannelElement* node = new ChannelElement;
	node->channel = channel;
	node->type = type;
	m_taskQ.push(node);
	m_mutex.unlock();
	if (m_threadID == this_thread::get_id()) {
		processTaskQ();
	}
	else {
		taskWakeup();
	}
	return 0;
}

int EventLoop::processTaskQ()
{
	while (!m_taskQ.empty()) {
		m_mutex.lock();
		ChannelElement* node = m_taskQ.front();
		m_taskQ.pop();
		m_mutex.unlock();
		Channel* channel = node->channel;
		if (node->type == ElemType::ADD) add(channel);
		else if (node->type == ElemType::DELETE) remove(channel);
		else if (node->type == ElemType::MODIFY) modify(channel);
		delete node;
	}
	return 0;
}

int EventLoop::add(Channel* channel)
{
	int fd = channel->getSocket();
	if (m_channelMap.find(fd) == m_channelMap.end()) {
		m_channelMap.insert(make_pair(fd, channel));
		m_dispatcher->setChannel(channel);
		int ret = m_dispatcher->add();
		return ret;
	}
	return -1;
}

int EventLoop::remove(Channel* channel)
{
	int fd = channel->getSocket();
	if (m_channelMap.find(fd) == m_channelMap.end()) {
		return -1;
	}
	m_dispatcher->setChannel(channel);
	int ret = m_dispatcher->remove();
	return ret;
}

int EventLoop::modify(Channel* channel)
{
	int fd = channel->getSocket();
	if (m_channelMap.find(fd) == m_channelMap.end()) return -1;
	m_dispatcher->setChannel(channel);
	int ret = m_dispatcher->modify();
	return ret;
}

int EventLoop::freeChannel(Channel* channel)
{
	auto it = m_channelMap.find(channel->getSocket());
	if (it != m_channelMap.end()) {
		m_channelMap.erase(it);
		close(channel->getSocket());
		delete channel;
	}
	return 0;
}

int EventLoop::readMessage()
{
	char buf[256];
	read(m_socketPair[1], buf, sizeof(buf));
	return 0;
}

int EventLoop::readLocalMessage(void* arg)
{
	EventLoop* evloop = static_cast<EventLoop*>(arg);
	char buf[256];
	read(evloop->m_socketPair[1], buf, sizeof(buf));
	return 0;
}

void EventLoop::taskWakeup()
{
	const char* msg = "My Server";
	write(m_socketPair[0], msg, strlen(msg));
}
