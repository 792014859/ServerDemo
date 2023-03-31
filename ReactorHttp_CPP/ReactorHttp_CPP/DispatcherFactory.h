#pragma once
#include"Dispatcher.h"
#include"SelectDispatcher.h"
#include"PollDispatcher.h"
#include"EpollDispatcher.h"
#include"EventLoop.h"



class DispatcherFactory
{
public:
	virtual Dispatcher* createDispatcher(EventLoop* evLoop) = 0;
	virtual ~DispatcherFactory() = default;
	DispatcherFactory() = default;
};

class SelectDispatcherFactory final : public DispatcherFactory 
{
public:
	SelectDispatcher* createDispatcher(EventLoop* evLoop) override {
		return new SelectDispatcher(evLoop);
	}
	SelectDispatcherFactory() = default;
	~SelectDispatcherFactory() = default;
};

class PollDispatcherFactory final : public DispatcherFactory
{
public:
	PollDispatcher* createDispatcher(EventLoop* evLoop) override {
		return new PollDispatcher(evLoop);
	}
	PollDispatcherFactory() = default;
	~PollDispatcherFactory() = default;
};

class EpollDispatcherFactory final : public DispatcherFactory
{
public:
	EpollDispatcher* createDispatcher(EventLoop* evLoop) override {
		return new EpollDispatcher(evLoop);
	}
	EpollDispatcherFactory() = default;
	~EpollDispatcherFactory() = default;
};

Dispatcher* getDispatcher(EventLoop* evLoop, int type) {
	DispatcherFactory* factory = nullptr;
	Dispatcher* result = nullptr;
	switch (type)
	{
	case 0:
		factory = new SelectDispatcherFactory;
		result = factory->createDispatcher(evLoop);
		break;
	case 1:
		factory = new PollDispatcherFactory;
		result = factory->createDispatcher(evLoop);
		break;
	default:
		factory = new EpollDispatcherFactory;
		result = factory->createDispatcher(evLoop);
		break;
	}
	return result;
}




