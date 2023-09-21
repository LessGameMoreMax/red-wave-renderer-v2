#ifndef TC_SPAN_LIST_H
#define TC_SPAN_LIST_H
#include "TCSpan.h"
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

    // inline TCSpan* GetBegin(){
    //     return span_head_->next_;
    // }
    //
    // inline TCSpan* GetEnd(){
    //     return span_head_;
    // }

    void PushFront(TCSpan* span);
    void PushBack(TCSpan* span);
    // TCSpan* TryPopFront();
    // TCSpan* TryPopBack();
    
    // void Insert(TCSpan* position, TCSpan* new_span);
    void Erase(TCSpan* position);

    // void Push(TCSpan* span);
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

    uint32_t RemoveRange(void** batch, uint32_t move_num);
    void InsertRange(void** batch, uint32_t move_num);
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
