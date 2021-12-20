#ifndef _THREAD_POOL_MANAGER_H_
#define _THREAD_POOL_MANAGER_H_

#include "Singleton.h"
#include "workthreadpool.h"
#include "NetManager.h"

class ThreadPoolManager : public WorkThreadPool
{
public:
	ThreadPoolManager(int size) : WorkThreadPool(size)
	{
	}

	void Handle(const std::string &msg);
	
	// int AddToThreadPool();
};

// #define sGLMgr ThreadPoolManager::getSingleton()

#endif