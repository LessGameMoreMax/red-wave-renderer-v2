#ifndef PRODUCER_CONSUMER_QUEUE_H
#define PRODUCER_CONSUMER_QUEUE_H
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include "../Core/Misc/MacroTools.h"
#include "../Core/Debug/Assertion.h"
namespace sablin{
namespace lenin{
template <typename T>
requires std::is_pointer_v<T>
struct PCNode{
    T data_;
    PCNode<T>* next_;
};
}

template<typename T>
requires std::is_pointer_v<T>
class ProducerConsumerQueue{
private:
    lenin::PCNode<T>* head_;
    lenin::PCNode<T>* tail_;
    std::mutex lock_;
    std::condition_variable cond_variable_;
    std::atomic<bool> has_stop_;
public:
    explicit ProducerConsumerQueue();
    CLASS_NO_ALLOWED_COPY(ProducerConsumerQueue)
    ~ProducerConsumerQueue();
    void PushBack(const T& value);
    T PopFrontWait(std::atomic<bool>& thread_has_stop);

    inline void NotifyAll(){
        cond_variable_.notify_all();
    }

    inline void NotifyOne(){
        cond_variable_.notify_one();
    }

    inline void Lock(){
        lock_.lock();
    }

    inline void UnLock(){
        lock_.unlock();
    }

    void Stop(bool should_wait);
};

template<typename T>
requires std::is_pointer_v<T>
ProducerConsumerQueue<T>::ProducerConsumerQueue():
        head_(nullptr), tail_(nullptr), has_stop_(false){
    lenin::PCNode<T>* temp = new lenin::PCNode<T>();
    temp->next_ = nullptr;
    head_ = tail_ = temp;
}

template<typename T>
requires std::is_pointer_v<T>
ProducerConsumerQueue<T>::~ProducerConsumerQueue(){
    ASSERT_WITH_STRING(has_stop_, "ProducerConsumerQueue Not Stop!")
    while(head_ != nullptr){
        lenin::PCNode<T>* temp = head_;
        head_ = head_->next_;
        delete temp;
    }
    head_ = tail_ = nullptr;
}

template<typename T>
requires std::is_pointer_v<T>
void ProducerConsumerQueue<T>::Stop(bool should_wait){
    if(has_stop_) return;
    has_stop_ = true;
    while(should_wait){
        lock_.lock();
        should_wait = (head_->next_ != nullptr);
        lock_.unlock();
        if(should_wait) std::this_thread::yield();
    }
}

template <typename T>
requires std::is_pointer_v<T>
void ProducerConsumerQueue<T>::PushBack(const T& value){
    if(has_stop_) {
        delete value;
        return;
    }
    lenin::PCNode<T>* temp = new lenin::PCNode<T>();
    ASSERT_NO_STRING(temp != nullptr)
    temp->data_ = value;
    temp->next_ = nullptr;

    lock_.lock();
    tail_->next_ = temp;
    tail_ = temp;
    lock_.unlock();

    NotifyOne();
}

template<typename T>
requires std::is_pointer_v<T>
T ProducerConsumerQueue<T>::PopFrontWait(std::atomic<bool>& thread_has_stop){
    std::unique_lock<std::mutex> lk(lock_);
    cond_variable_.wait(lk, [&]{
        return head_->next_ != nullptr || thread_has_stop;
    });
    if(thread_has_stop) return nullptr;
    lenin::PCNode<T>* temp = head_;
    T result = head_->next_->data_;
    head_ = head_->next_;
    lk.unlock();

    delete temp;
    return std::move(result);
}

}
#endif
