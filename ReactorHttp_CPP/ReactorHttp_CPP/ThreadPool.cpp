#include "ThreadPool.h"
#include <assert.h>
#include <thread>

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
	for (auto item : m_workerThreads) delete item;
}

void ThreadPool::run()
{
	assert(!m_isStart);
	if (m_mainloop->getThreadID() != this_thread::get_id()) {
		exit(0);
	}
	m_isStart = true;
	if (m_threadNum > 0) {
		for (int i = 0; i < m_threadNum; ++i) {
			WorkerThread* subThread = new WorkerThread(i);
			subThread->run();
			m_workerThreads.push_back(subThread);
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
