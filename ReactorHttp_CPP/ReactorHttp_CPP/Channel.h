#pragma once
#include<functional>

//定义文件描述符的读写事件
enum class FDEvent {
	Timeout = 0x01,
	ReadEvent = 0x02,
	WriteEvent = 0x04
};

class Channel
{
public:
	using handleFunc = std::function<int(void*)>;
	Channel(int _fd, FDEvent _events, handleFunc _readFunc, handleFunc _writeFunc, handleFunc _destroyFunc, void* _arg);

	//修改fd的写事件(检测 or 不检测)
	void writeEventEnable(bool flag);

	//判读是否需要检测文件描述符的写事件
	bool isWriteEventEnable();

	inline int getEvent() { return m_events; }
	inline int getSocket() { return m_fd; }
	inline const void* getArg() { return m_arg; }


	//回调函数
	handleFunc readCallback;
	handleFunc writeCallback;
	handleFunc destroyCallback;

private:
	//文件描述符
	int m_fd;
	//事件
	int m_events;
	//回调函数的参数
	void* m_arg;
};

