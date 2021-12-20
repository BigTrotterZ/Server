#ifndef __WORK_THREAD_POOL__
#define __WORK_THREAD_POOL__

// #include <stdio.h>
#include <thread>
#include <queue>
#include <string>
#include <vector>
#include "lckfree.h"

class WorkThreadPool
{
private:
    void Worker();

    int size_;
    LckFreeQueue msg_queue_;
    std::vector<std::thread> thread_pool_;
public:
    WorkThreadPool(int size = 1);
    virtual ~WorkThreadPool();

    virtual void Init() {};
    virtual void Finish() {};
    virtual void Handle(const std::string &msg) = 0;

    int SendMessage(const std::string &msg);
    int Start();
    int Stop();
};




#endif