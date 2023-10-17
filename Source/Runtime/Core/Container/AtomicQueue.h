#ifndef ATOMIC_QUEUE_H
#define ATOMIC_QUEUE_H
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include "../Misc/MacroTools.h"
#include "../Misc/TypeTraits.h"
namespace sablin{

template <typename T>
class AtomicQueue{
private:
    std::mutex lock_;
    std::condition_variable cond_variable_;
    std::queue<T> queue_;
public:
    AtomicQueue() = default;
    ~AtomicQueue();
    CLASS_NO_ALLOWED_COPY(AtomicQueue)

    bool IsEmpty(){
        std::lock_guard<std::mutex> lk(lock_);
        return queue_.empty();
    }

    void Push(T&& value){
        while(true){
            if(lock_.try_lock()){
                queue_.push(std::move(value));
                lock_.unlock();
                break;
            }else{
                std::this_thread::yield();
            }
        }
        cond_variable_.notify_one();
    }

    T TryPop(){
        std::lock_guard<std::mutex> lk(lock_);
    }



};

}
#endif
