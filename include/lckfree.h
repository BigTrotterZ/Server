#ifndef __LCKFREE_H__
#define __LCKFREE_H__

#include <string>
struct LinkNode
{
    std::string data;
    LinkNode * next;
};
typedef struct LinkNode LinkNode;

class LckFreeQueue
{
private:
    LinkNode * head_;
    LinkNode * tail_;
    bool empty_;
    unsigned int length_;
public:
    LckFreeQueue();
    ~LckFreeQueue();

    int push(const std::string &msg);
    std::string pop();
    bool empty();
};


#endif