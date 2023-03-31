#pragma once
#include "EventLoop.h"
#include "WorkerThread.h"
#include <vector>
class ThreadPool
{
public:
	ThreadPool(EventLoop* mainLoop, int count);
	~ThreadPool();
	//�����̳߳�
	void run(const int dispatcherType);
	//�����߳���ȡ��һ����Ӧ��ʵ��
	EventLoop* takeWorkerEventLoop();
private:
	//ָ������Ӧ��ʵ��
	EventLoop* m_mainloop;
	//����̳߳��Ƿ�����
	bool m_isStart;
	//�̳߳������̸߳���
	int m_threadNum;
	//������߳�
	std::vector<WorkerThread*> m_workerThreads;
	//���ڱ�Ǵ��Ǹ����߳���ȡ����Ӧ��ʵ������Ӧm_workerThreads���±�
	int m_index;
};
