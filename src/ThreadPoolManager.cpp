#include "ThreadPoolManager.h"
#include "zzLog.h"
// #include "Server.h"


void ThreadPoolManager::Handle(const std::string &msg)
{
    if(msg == "__net_work__")
    {
        sNetMgr.Loop();
        return;
    }
}