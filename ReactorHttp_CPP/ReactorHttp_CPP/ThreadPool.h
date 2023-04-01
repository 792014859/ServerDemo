#pragma once
#include "EventLoop.h"
#include "WorkerThread.h"
#include <vector>
class ThreadPool
{
public:
	ThreadPool(EventLoop* mainLoop, int count);
	~ThreadPool();
	//启动线程池
	void run(const int dispatcherType);
	//从子线程中取出一个反应堆实例
	EventLoop* takeWorkerEventLoop();
private:
	//指向主反应堆实例
	EventLoop* m_mainloop;
	//存放子线程
	std::vector<WorkerThread*> m_workerThreads;
	//标记线程池是否启动
	bool m_isStart;
	//线程池中子线程个数
	int m_threadNum;
	//用于标记从那个子线程中取出反应堆实例，对应m_workerThreads的下标
	int m_index;
};

