#ifndef ATOMIC_PRIORITY_QUEUE_H
#define ATOMIC_PRIORITY_QUEUE_H
#include <type_traits>
#include <queue>
#include <mutex>
#include "Macro/MacroTools.h"
#include "Debug/Assertion.h"
namespace sablin{

template <typename T, typename P>
requires std::is_pointer_v<T>
class AtomicPriorityQueue{
private:
    struct Element{
        T pointer_;
        P priority_;

        friend bool operator<(const Element& lhs, const Element& rhs){
            return lhs.priority_ < rhs.priority_;
        }
    };

    std::mutex lock_;
    std::priority_queue<Element> priority_queue_;
public:
    AtomicPriorityQueue() = default;
    ~AtomicPriorityQueue(){
        ASSERT_NO_STRING(IsEmpty())
    }
    CLASS_NO_ALLOWED_COPY(AtomicPriorityQueue)

    bool IsEmpty(){
        std::lock_guard<std::mutex> lk(lock_);
        return priority_queue_.empty();
    }

    void Push(T pointer, P priority){
        std::lock_guard<std::mutex> lk(lock_);
        priority_queue_.push({std::move(pointer), priority});
    }

    //No Any Wait!
    T TryPop(){
        T result = nullptr;
        if(!priority_queue_.empty() && lock_.try_lock()){
            if(!priority_queue_.empty()){
                result = priority_queue_.top().pointer_;
                priority_queue_.pop();
            }
            lock_.unlock();
        }
        return result;
    }

    bool TryPopBatch(std::vector<T>& batch, int32_t max_pool_batch_size){
        bool result = false;
        if(lock_.try_lock()){
            while(!priority_queue_.empty() && max_pool_batch_size-- >0){
                batch.emplace_back(priority_queue_.top().pointer_);
                priority_queue_.pop();
                result = true;
            }
            lock_.unlock();
        }
        return result;
    }
};

}
#endif
