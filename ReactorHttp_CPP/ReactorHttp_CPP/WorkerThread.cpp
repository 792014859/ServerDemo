#include "WorkerThread.h"
using namespace std;
WorkerThread::WorkerThread(int index)
{
	m_evLoop = nullptr;
	m_thread = nullptr;
	//m_threadID = thread::id(); //初始化
	m_name = "SubThread-" + to_string(index);
}

WorkerThread::~WorkerThread()
{
	if (m_thread != nullptr) delete m_thread;
}

void WorkerThread::run(const int dispatcherType) //该函数由主线程调用
{
	//创建子线程
	m_thread = new thread(&WorkerThread::running, this, dispatcherType);
	//阻塞主线程，让当前函数不会直接结束
	unique_lock<mutex> locker(m_mutex);
	//m_cond.wait(locker, [this]() {return m_evLoop != nullptr; });
	while (m_evLoop == nullptr) m_cond.wait(locker);
}

void WorkerThread::running(const int dispatcherType)
{
	m_mutex.lock();
	m_evLoop = new EventLoop(m_name, dispatcherType);
	m_mutex.unlock();
	m_cond.notify_one();
	m_evLoop->run();
}
