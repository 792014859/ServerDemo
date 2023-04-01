#include "ThreadPool.h"
#include <assert.h>
#include <thread>
#include <iostream>

using namespace std;

ThreadPool::ThreadPool(EventLoop* mainLoop, int count)
{
	m_index = 0;
	m_isStart = false;
	m_mainloop = mainLoop;
	m_threadNum = count;
	m_workerThreads.clear();
}

ThreadPool::~ThreadPool()
{
	for (auto item : m_workerThreads) {
		if (item != nullptr) {
			if (item->m_thread->joinable()) item->m_thread->join();
			delete item;
			item = nullptr;
		}
		
	}
}

void ThreadPool::run(const int dispatcherType)
{
	assert(!m_isStart);
	if (m_mainloop->getThreadID() != this_thread::get_id()) {
		exit(0);
	}
	m_isStart = true;
	if (m_threadNum > 0) {
		m_workerThreads = std::vector<WorkerThread*>(m_threadNum, nullptr);
		for (int i = 0; i < m_threadNum; ++i) {
			m_workerThreads[i] = new WorkerThread(i);
			m_workerThreads[i]->run(dispatcherType);
		}
	}
}

EventLoop* ThreadPool::takeWorkerEventLoop()
{
	assert(m_isStart);
	if (m_mainloop->getThreadID() != this_thread::get_id()) exit(0);
	EventLoop* evLoop = m_mainloop;
	if (m_threadNum > 0) {
		evLoop = m_workerThreads[m_index]->getEventLoop();
		m_index = ++m_index % m_threadNum;
	}
	return evLoop;
}
