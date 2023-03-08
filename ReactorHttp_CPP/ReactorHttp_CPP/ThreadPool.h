#pragma once
#include "EventLoop.h"
#include "WorkerThread.h"
#include <vector>
class ThreadPool
{
public:
	ThreadPool(EventLoop* mainLoop, int count);
	~ThreadPool();
	void run();
	EventLoop* takeWorkerEventLoop();
private:
	EventLoop* m_mainloop;
	bool m_isStart;
	int m_threadNum;
	std::vector<WorkerThread*> m_workerThreads;
	int m_index;
};

