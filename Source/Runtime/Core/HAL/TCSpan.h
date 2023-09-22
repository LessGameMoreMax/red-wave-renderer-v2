#ifndef TC_SPAN_H
#define TC_SPAN_H
#include "TCPageId.h"
#include "../Debug/Assertion.h"
#include <mutex>
#include <atomic>
#include "MallocBase.h"
namespace sablin{

class TCSpanList;

class TCSpan: public UseSystemMallocForNew{
private:
    PageId first_page_id_;
    uintptr_t pages_num_;
    std::size_t object_size_;
    std::size_t max_allocated_num_;
    std::size_t allocated_num_;
    void* free_list_;
    std::mutex lock_;
public:
    TCSpan* prev_;
    TCSpan* next_;
private:
    inline static void* GetNextPtr(void* ptr){
        return *(reinterpret_cast<void**>(ptr));
    }

    inline static void SetNextPtr(void* ptr, void* next_ptr){
        *(reinterpret_cast<void**>(ptr)) = next_ptr;
    }
public:
    TCSpan() = default;
    ~TCSpan() = default;
    inline PageId GetFirstPageId() const;
    inline PageId GetLastPageId() const;
    inline void SetFirstPageId(PageId page_id);
    inline void* GetStartAddr() const;
    inline void* GetEndAddr() const;
    inline uintptr_t GetPageNum() const;
    inline void SetPageNum(uintptr_t num);
    inline std::size_t GetSpanSize() const;
    inline std::size_t GetAllocatedSize() const;
    inline bool TryLock();
    inline void UnLock();
    inline void Lock();

    inline bool IsFull() const;
    inline bool IsEmpty() const;

    void Initialize(PageId first_page_id, uintptr_t pages_num);

    inline void Push(void* ptr){
        SetNextPtr(ptr, free_list_);
        free_list_ = ptr;
        --allocated_num_;
    }

    inline void InitializePush(void* ptr){
        SetNextPtr(ptr, free_list_);
        free_list_ = ptr;
    }

    inline void* Pop(){
        void* result = free_list_;
        free_list_ = GetNextPtr(free_list_);
        ++allocated_num_;
        return result;
    }

    void SetObjectSize(std::size_t object_size);
    inline std::size_t GetObjectSize() const{
        return object_size_;
    }
};

inline PageId TCSpan::GetFirstPageId() const{
    return first_page_id_;
}

inline PageId TCSpan::GetLastPageId() const{
    return first_page_id_ + pages_num_ - 1;
}

inline void TCSpan::SetFirstPageId(PageId page_id){
#ifdef DEBUG
    ASSERT_WITH_STRING(page_id > PageId(0), "TCSpan::SetFirstPageId: page_id <= 0")
#endif
    first_page_id_ = page_id;
}

inline void* TCSpan::GetStartAddr() const{
#ifdef DEBUG
    ASSERT_WITH_STRING(first_page_id_ > PageId(0), "TCSpan::GetStartAddr: page_id <= 0")
#endif
    return first_page_id_.GetStartAddr();
}

inline void* TCSpan::GetEndAddr() const{
    return (first_page_id_ + pages_num_).GetStartAddr();
}

inline uintptr_t TCSpan::GetPageNum() const{
    return pages_num_;
}

inline void TCSpan::SetPageNum(uintptr_t num){
    pages_num_ = num;
}

inline std::size_t TCSpan::GetSpanSize() const{
    return pages_num_ * kPageSize;
}

inline std::size_t TCSpan::GetAllocatedSize() const{
    return allocated_num_ * object_size_;
}

inline bool TCSpan::IsFull() const{
    return allocated_num_ == max_allocated_num_;
}

inline bool TCSpan::IsEmpty() const{
    return allocated_num_ == 0;
}

inline void TCSpan::Lock(){
    lock_.lock();
}

inline bool TCSpan::TryLock(){
    return lock_.try_lock();
}

inline void TCSpan::UnLock(){
    lock_.unlock();
}
}
#endif
