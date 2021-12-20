#include "lckfree.h"

LckFreeQueue::LckFreeQueue(): head_(NULL), tail_(NULL), empty_(true), length_(0)
{
    head_ = new LinkNode;
    head_->next = NULL;
    tail_ = head_;
}

LckFreeQueue::~LckFreeQueue()
{
    LinkNode * p = head_;
    if(p)
    {
        LinkNode * q = p->next;
        delete p;
        p = q;
    }
}

int LckFreeQueue::push(const std::string &msg)
{
    LinkNode * q = new LinkNode;
    q->data = msg;
    q->next = NULL;

    LinkNode * p = tail_;
    LinkNode * old_p = p;
    do
    {
        while(p->next != NULL)
            p = p->next;
    } while (__sync_bool_compare_and_swap(&(p->next), NULL, q) != true);
    
    __sync_bool_compare_and_swap(&tail_, old_p, q);
    empty_ = false;
    return 0;
}

std::string LckFreeQueue::pop()
{
    LinkNode * p;
    do
    {
        p = head_;
        if(p->next == NULL)
        {
            empty_ = true;
            return "";
        }
    } while (__sync_bool_compare_and_swap(&head_, p, p->next) != true);
    
    return p->next->data;
}

bool LckFreeQueue::empty()
{
    return empty_;
}