#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H
#include <mutex>
#include <thread>
#include <optional>
#include "Macro/MacroTools.h"
#include "Debug/Assertion.h"
namespace sablin{
namespace lenin{
template <typename T>
struct QueueNode{
    T data_;
    QueueNode<T>* next_;
};
}

template<typename T>
class ThreadSafeQueue{
private:
    lenin::QueueNode<T>* head_;
    lenin::QueueNode<T>* tail_;
    std::mutex head_lock_;
    std::mutex tail_lock_;
public:
    explicit ThreadSafeQueue();
    CLASS_NO_ALLOWED_COPY(ThreadSafeQueue)
    ~ThreadSafeQueue();

    void PushBack(const T& value);
    std::optional<T> PopFront();
};

template <typename T>
ThreadSafeQueue<T>::ThreadSafeQueue(): head_(nullptr), tail_(nullptr){
    lenin::QueueNode<T>* temp = new lenin::QueueNode<T>();
    temp->next_ = nullptr;
    head_ = tail_ = temp;
}

template <typename T>
ThreadSafeQueue<T>::~ThreadSafeQueue(){
    while(head_ != nullptr){
        lenin::QueueNode<T>* temp = head_;
        head_ = head_->next_;
        delete temp;
    }
    head_ = tail_ = nullptr;
}

template <typename T>
void ThreadSafeQueue<T>::PushBack(const T& value){
    lenin::QueueNode<T>* temp = new lenin::QueueNode<T>();
    ASSERT_NO_STRING(temp != nullptr)
    temp->data_ = value;
    temp->next_ = nullptr;

    tail_lock_.lock();
    tail_->next_ = temp;
    tail_ = temp;
    tail_lock_.unlock();
}

template <typename T>
std::optional<T> ThreadSafeQueue<T>::PopFront(){
    head_lock_.lock();
    lenin::QueueNode<T>* temp = head_;
    lenin::QueueNode<T>* new_head = temp->next_;
    if(new_head == nullptr){
        head_lock_.unlock();
        return std::nullopt;
    }
    std::optional<T> result = new_head->data_;
    head_ = new_head;
    head_lock_.unlock();
    delete temp;
    return std::move(result);
}
}
#endif
