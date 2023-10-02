#ifndef TC_SPAN_LIST_H
#define TC_SPAN_LIST_H
#include "TCSpan.h"
#include "../Misc/MacroDefine.h"
namespace sablin{

class TCSpanList{
private:
    TCSpan* span_head_;
    std::mutex lock_;
public:
    TCSpanList();
    ~TCSpanList();

    void Clear();

    TCSpanList(const TCSpanList&) = delete;
    TCSpanList& operator=(const TCSpanList&) = delete;

    void PushFront(TCSpan* span);
    void PushBack(TCSpan* span);
    
    void Erase(TCSpan* position);

    TCSpan* TryPop();

    inline TCSpan* Begin(){
        return span_head_->next_;
    }

    inline bool IsEmpty(){
        return span_head_->next_ == span_head_;
    }

    inline bool TryLock();
    inline void Lock();
    inline void UnLock();

#ifdef DEBUG
public:
    uint64_t length_ = 0;
#endif
};

inline bool TCSpanList::TryLock(){
    return lock_.try_lock();
}

inline void TCSpanList::Lock(){
    lock_.lock();
}

inline void TCSpanList::UnLock(){
    lock_.unlock();
}

}
#endif
