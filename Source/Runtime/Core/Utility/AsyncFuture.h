#ifndef ASYNC_FUTURE_H
#define ASYNC_FUTURE_H
#include <future>
#include "../Misc/MacroTools.h"
#include "../HAL/Runnable.h"
#include "../HAL/RunnableThread.h"

namespace sablin{

template<typename T>
class AsyncFuture{
private:
    std::future<T> future_;
    Runnable* runnable_;
    RunnableThread* runnable_thread_;
public:
    explicit AsyncFuture(): runnable_(nullptr), runnable_thread_(nullptr){}
    explicit AsyncFuture(std::future<T>&& future, Runnable* runnable, RunnableThread* runnable_thread):
        future_(std::move(future)), runnable_(runnable), runnable_thread_(runnable_thread){}

    AsyncFuture(AsyncFuture&& other):
        future_(std::move(other.future_)), 
        runnable_(other.runnable_), 
        runnable_thread_(other.runnable_thread_){
        other.runnable_ = nullptr;
        other.runnable_thread_ = nullptr;
    }
    CLASS_NO_ALLOWED_COPY(AsyncFuture)

    AsyncFuture& operator=(AsyncFuture&& other){
        Exit();
        future_ = std::move(other.future_);
        runnable_ = other.runnable_;
        runnable_thread_ = other.runnable_thread_;
        other.runnable_ = nullptr;
        other.runnable_thread_ = nullptr;
        return *this;
    }

    ~AsyncFuture(){ Exit(); }


    void Exit(){
        if(runnable_thread_ != nullptr){
            delete runnable_thread_;
            runnable_thread_ = nullptr;
        }
        if(runnable_ != nullptr){
            delete runnable_;
            runnable_ = nullptr;
        }
    }

    T Get(){
        return future_.get();
    }

    void Wait() const{
        future_.wait();
    }
};

template<>
class AsyncFuture<void>{
private:
    std::future<void> future_;
    Runnable* runnable_;
    RunnableThread* runnable_thread_;
public:
    explicit AsyncFuture(): runnable_(nullptr), runnable_thread_(nullptr){}
    explicit AsyncFuture(std::future<void>&& future, Runnable* runnable, RunnableThread* runnable_thread):
        future_(std::move(future)), runnable_(runnable), runnable_thread_(runnable_thread){}

    AsyncFuture(AsyncFuture&& other):
        future_(std::move(other.future_)), 
        runnable_(other.runnable_), 
        runnable_thread_(other.runnable_thread_){
        other.runnable_ = nullptr;
        other.runnable_thread_ = nullptr;
    }
    CLASS_NO_ALLOWED_COPY(AsyncFuture)

    AsyncFuture& operator=(AsyncFuture&& other){
        Exit();
        future_ = std::move(other.future_);
        runnable_ = other.runnable_;
        runnable_thread_ = other.runnable_thread_;
        other.runnable_ = nullptr;
        other.runnable_thread_ = nullptr;
        return *this;
    }

    ~AsyncFuture(){ Exit(); }


    void Exit(){
        if(runnable_thread_ != nullptr){
            delete runnable_thread_;
            runnable_thread_ = nullptr;
        }
        if(runnable_ != nullptr){
            delete runnable_;
            runnable_ = nullptr;
        }
    }

    void Get(){
        future_.get();
    }

    void Wait() const{
        future_.wait();
    }
};

template<typename T>
class AsyncFuture<T&>{
private:
    std::future<T&> future_;
    Runnable* runnable_;
    RunnableThread* runnable_thread_;
public:
    explicit AsyncFuture(): runnable_(nullptr), runnable_thread_(nullptr){}
    explicit AsyncFuture(std::future<T&>&& future, Runnable* runnable, RunnableThread* runnable_thread):
        future_(std::move(future)), runnable_(runnable), runnable_thread_(runnable_thread){}

    AsyncFuture(AsyncFuture&& other):
        future_(std::move(other.future_)), 
        runnable_(other.runnable_), 
        runnable_thread_(other.runnable_thread_){
        other.runnable_ = nullptr;
        other.runnable_thread_ = nullptr;
    }
    CLASS_NO_ALLOWED_COPY(AsyncFuture)

    AsyncFuture& operator=(AsyncFuture&& other){
        Exit();
        future_ = std::move(other.future_);
        runnable_ = other.runnable_;
        runnable_thread_ = other.runnable_thread_;
        other.runnable_ = nullptr;
        other.runnable_thread_ = nullptr;
        return *this;
    }

    ~AsyncFuture(){ Exit(); }


    void Exit(){
        if(runnable_thread_ != nullptr){
            delete runnable_thread_;
            runnable_thread_ = nullptr;
        }
        if(runnable_ != nullptr){
            delete runnable_;
            runnable_ = nullptr;
        }
    }

    T& Get(){
        return future_.get();
    }

    void Wait() const{
        future_.wait();
    }
};

}
#endif
