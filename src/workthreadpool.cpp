#include <unistd.h>
#include <sstream>
#include <iostream>
#include "workthreadpool.h"

WorkThreadPool::WorkThreadPool(int size)
{
    if(size <= 0)
        size_ = 1;
    else
    {
        size_ = size;
    }
    
}

WorkThreadPool::~WorkThreadPool()
{
}

int WorkThreadPool::SendMessage(const std::string &msg)
{
    msg_queue_.push(msg);
    return 0;
}

void WorkThreadPool::Worker()
{
    unsigned int msg_count = 0;
    while(true)
    {
        std::string msg = msg_queue_.pop();
        if(msg.empty())
        {
            std::cout << "no msg got, sleep for 0.1 sec" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        if(msg == "__exit__")
        {
            std::stringstream ss;
            ss << "exit worker: " << std::this_thread::get_id() << ", processed: " << msg_count << "..";
            std::cout << ss << std::endl;
            return;
        }
        Handle(msg);
        msg_count++;
        if(msg_count % 1000 == 0)
        {
            std::cout << "every 1000 msg count" << std::endl;
        }
    }
}

int WorkThreadPool::Start()
{
    for(int i=0; i < size_; ++i)
    {
        thread_pool_.push_back(std::thread(&WorkThreadPool::Worker, this));
    }
    return 0;
}

int WorkThreadPool::Stop()
{
    for(int i=0; i < size_; ++i)
    {
        SendMessage("__exit__");
    }
    for(int i=0; i < size_; ++i)
    {
        thread_pool_[i].join();
    }
    return 0;
}