#ifndef PRODUCER_CONSUMER_QUEUE_H
#define PRODUCER_CONSUMER_QUEUE_H
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <list>
#include "Macro/MacroTools.h"
#include "Debug/Assertion.h"
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
    std::atomic<uint32_t> size_;
private:
    inline void NotifyAll(){
        cond_variable_.notify_all();
    }

    inline void NotifyOne(){
        cond_variable_.notify_one();
    }
public:
    explicit ProducerConsumerQueue();
    CLASS_NO_ALLOWED_COPY(ProducerConsumerQueue)
    ~ProducerConsumerQueue();
    bool PushBack(const T& value);
    T PopFront();
    std::list<T> PopFrontBatch(uint32_t max_num);
    T TryPopFront();
    std::list<T> TryPopFrontBatch(uint32_t max_num);
    T PopFrontUntil(const std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds>& tp);


    inline void Lock(){
        lock_.lock();
    }

    inline void UnLock(){
        lock_.unlock();
    }

    void Stop();
    bool IsEmpty();

    bool HasStop() const;
    uint32_t GetRoughSize() const;
};

template<typename T>
requires std::is_pointer_v<T>
ProducerConsumerQueue<T>::ProducerConsumerQueue():
        head_(nullptr), 
        tail_(nullptr), 
        has_stop_(false),
        size_(0){
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
    size_ = 0;
}

template<typename T>
requires std::is_pointer_v<T>
void ProducerConsumerQueue<T>::Stop(){
    std::unique_lock<std::mutex> lk(lock_);
    has_stop_ = true;
    lk.unlock();
    NotifyAll();
}

template<typename T>
requires std::is_pointer_v<T>
bool ProducerConsumerQueue<T>::IsEmpty(){
    std::unique_lock<std::mutex> lk(lock_);
    return head_->next_ == nullptr;
}

template<typename T>
requires std::is_pointer_v<T>
bool ProducerConsumerQueue<T>::HasStop() const{
    return has_stop_;
}

template<typename T>
requires std::is_pointer_v<T>
uint32_t ProducerConsumerQueue<T>::GetRoughSize() const{
    return size_;
}

template <typename T>
requires std::is_pointer_v<T>
bool ProducerConsumerQueue<T>::PushBack(const T& value){
    if(has_stop_) { return false;}
    lenin::PCNode<T>* temp = new lenin::PCNode<T>();
    ASSERT_NO_STRING(temp != nullptr)
    temp->data_ = value;
    temp->next_ = nullptr;

    lock_.lock();
    tail_->next_ = temp;
    tail_ = temp;
    lock_.unlock();

    ++size_;
    NotifyOne();
    return true;
}

template<typename T>
requires std::is_pointer_v<T>
T ProducerConsumerQueue<T>::PopFront(){
    std::unique_lock<std::mutex> lk(lock_);
    cond_variable_.wait(lk, [&]{
        return head_->next_ != nullptr || has_stop_;
    });
    if(has_stop_) return nullptr;
    lenin::PCNode<T>* temp = head_;
    T result = head_->next_->data_;
    head_ = head_->next_;
    lk.unlock();

    delete temp;
    --size_;
    return std::move(result);
}

template<typename T>
requires std::is_pointer_v<T>
std::list<T> ProducerConsumerQueue<T>::PopFrontBatch(uint32_t max_num){
    std::unique_lock<std::mutex> lk(lock_);
    cond_variable_.wait(lk, [&]{
        return head_->next_ != nullptr || has_stop_;
    });
    std::list<T> result;
    if(has_stop_) return result;
    while(head_->next_ != nullptr && max_num != 0){
        lenin::PCNode<T>* temp = head_;
        result.push_back(head_->next_->data_);
        head_ = head_->next_;
        delete temp;
        --max_num;
        --size_;
    }
    lk.unlock();

    return result;
}

template<typename T>
requires std::is_pointer_v<T>
T ProducerConsumerQueue<T>::TryPopFront(){
    if(!lock_.try_lock()) return nullptr;
    std::unique_lock<std::mutex> lk(lock_, std::adopt_lock);
    if(head_->next_ == nullptr || has_stop_) return nullptr;

    lenin::PCNode<T>* temp = head_;
    T result = head_->next_->data_;
    head_ = head_->next_;
    lk.unlock();

    delete temp;
    --size_;
    return std::move(result);
}

template<typename T>
requires std::is_pointer_v<T>
std::list<T> ProducerConsumerQueue<T>::TryPopFrontBatch(uint32_t max_num){
    std::list<T> result;
    if(!lock_.try_lock()) return result;
    std::unique_lock<std::mutex> lk(lock_, std::adopt_lock);
    if(head_->next_ == nullptr || has_stop_) return result;

    while(head_->next_ != nullptr && max_num != 0){
        lenin::PCNode<T>* temp = head_;
        result.push_back(head_->next_->data_);
        head_ = head_->next_;
        delete temp;
        --max_num;
        --size_;
    }
    lk.unlock();

    return result;
}

template<typename T>
requires std::is_pointer_v<T>
T ProducerConsumerQueue<T>::PopFrontUntil(const std::chrono::time_point<std::chrono::steady_clock, 
        std::chrono::milliseconds>& timepoint){
    std::unique_lock<std::mutex> lk(lock_);
    cond_variable_.wait_until(lk, timepoint, [&]{
        return head_->next_ != nullptr || has_stop_;
    });
    if(has_stop_) return nullptr;
    if(head_->next_ == nullptr) return nullptr;

    lenin::PCNode<T>* temp = head_;
    T result = head_->next_->data_;
    head_ = head_->next_;
    lk.unlock();

    delete temp;
    --size_;
    return std::move(result);
}
}
#endif
