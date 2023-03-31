#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include "EventLoop.h"
class WorkerThread
{
public:
	WorkerThread(int index);
	~WorkerThread();
	void run(const int dispatcherType);
	inline EventLoop* getEventLoop() { return m_evLoop; }
private:
	void running(const int dispatcherType);

private:
	std::thread* m_thread; //保存线程的实例
	//std::thread::id m_threadID; //线程id
	std::string m_name; 
	std::mutex m_mutex;
	std::condition_variable m_cond;
	EventLoop* m_evLoop; // 反应堆模型
};

