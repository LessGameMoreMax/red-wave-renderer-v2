#ifndef ATOMIC_QUEUE_H
#define ATOMIC_QUEUE_H
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include "../Core/Misc/MacroTools.h"
#include "../Core/Debug/Assertion.h"
namespace sablin{

template <typename T>
requires std::is_pointer_v<T>
class AtomicQueue{
private:
    std::mutex lock_;
    std::condition_variable cond_variable_;
    std::queue<T> queue_;
public:
    AtomicQueue() = default;
    ~AtomicQueue(){
        ASSERT_NO_STRING(IsEmpty())
    }
    CLASS_NO_ALLOWED_COPY(AtomicQueue)

    bool IsEmpty(){
        std::lock_guard<std::mutex> lk(lock_);
        return queue_.empty();
    }

    void Push(T pointer){
        while(true){
            if(lock_.try_lock()){
                queue_.push(std::move(pointer));
                lock_.unlock();
                break;
            }else{
                std::this_thread::yield();
            }
        }
        cond_variable_.notify_one();
    }

    // Wait For Lock!
    T Pop(){
        std::lock_guard<std::mutex> lk(lock_);
        if(queue_.empty()) {
            return nullptr;
        }
        T result = std::move(queue_.front());
        queue_.pop();
        return result;
    }

    T PopTimeOut(long ms){
        std::unique_lock<std::mutex> lk(lock_);
        if(!cond_variable_.wait_for(lk, std::chrono::milliseconds(ms), [this]{ return !queue_.empty();})){
            return nullptr;
        }

        T result = std::move(queue_.front());
        queue_.pop();
        return result;
    }

    bool TryPopBatch(std::vector<T>& batch, int32_t max_pool_batch_size){
        bool result = false;
        if(!queue_.empty() && lock_.try_lock()){
            while(!queue_.empty() && max_pool_batch_size-- > 0){
                batch.emplace_back(std::move(queue_.front()));
                queue_.pop();
                result = true;
            }
            lock_.unlock();
        }
        return result;
    }

    // No Any Wait!
    T TryPop(){
        T result = nullptr;
        if(!queue_.empty() && lock_.try_lock()){
            if(!queue_.empty()){ // Important!
                result = std::move(queue_.front());
                queue_.pop();
            }
            lock_.unlock();
        }
        return result;
    }

    // Wait For Lock And Element!
    T WaitPop(){
        std::lock_guard<std::mutex> lk(lock_);
        while(queue_.empty())
            cond_variable_.wait(lk, [this]{ return !queue_.empty();});
        T result = std::move(queue_.front());
        queue_.pop();
        return result;
    }
};

}
#endif
