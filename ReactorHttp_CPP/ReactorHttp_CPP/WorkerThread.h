#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <memory>
#include "EventLoop.h"

namespace zxd {
	#if __cplusplus >= 201402L
	using std::make_unique;
	#else 
	template <typename T, typename... Args>
	std::unique_ptr<T> make_unique(Args&&... args) {
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
	#endif
}


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
	std::unique_ptr<std::thread> m_thread; //保存线程的实例
	std::string m_name; 
	std::mutex m_mutex;
	std::condition_variable m_cond;
	EventLoop* m_evLoop; // 反应堆模型

	friend class ThreadPool;
};

