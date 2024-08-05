#ifndef WORK_FUTURE_H
#define WORK_FUTURE_H
#include "Macro/MacroTools.h"
#include "Core/Memory/MemoryBase.h"
#include <memory>
#include <type_traits>
namespace sablin{

template<typename R>
concept IsReturnType = std::is_void_v<R> || std::is_trivially_default_constructible_v<R>;

template<IsReturnType R>
class WorkPromise;

namespace lenin{

template<typename R>
class WorkFutureImpl{
private:
    std::shared_ptr<R> result_;
    std::mutex lock_;
public:
    explicit WorkFutureImpl(std::shared_ptr<R> result): result_(result){
        Lock();
    }

    R Get(){
        std::lock_guard<std::mutex> lk(lock_);
        return *result_;
    }

    inline void Lock(){
        lock_.lock();
    }

    inline void UnLock(){
        lock_.unlock();
    }
};

template<>
class WorkFutureImpl<void>{
private:
    std::mutex lock_;
public:
    explicit WorkFutureImpl(){ Lock();}

    void Get(){
        std::lock_guard<std::mutex> lk(lock_);
    }

    inline void Lock(){
        lock_.lock();
    }

    inline void UnLock(){
        lock_.unlock();
    }
};

}


template<typename R>
class WorkSharedFuture{
    friend class WorkPromise<R>;
private:
    std::shared_ptr<lenin::WorkFutureImpl<R>> future_;
private:
    explicit WorkSharedFuture(const std::shared_ptr<lenin::WorkFutureImpl<R>> future): 
        future_(future){}
public:
    ~WorkSharedFuture() = default;

    R Get(){
        return future_->Get();
    }
};

template<IsReturnType R>
class WorkPromise{
private:
    std::shared_ptr<R> result_;
    std::shared_ptr<lenin::WorkFutureImpl<R>> future_;
public:
    explicit WorkPromise(){
        result_ = std::make_shared<R>();
        future_ = std::make_shared<lenin::WorkFutureImpl<R>>(result_);
    }
    ~WorkPromise() = default;

    void SetValue(R&& value){
        *result_ = std::move(value);
        future_->UnLock();
    }

    WorkSharedFuture<R> GetSharedFuture(){
        return WorkSharedFuture(future_);
    }
};


template<>
class WorkSharedFuture<void>{
    friend class WorkPromise<void>;
private:
    std::shared_ptr<lenin::WorkFutureImpl<void>> future_;
private:
    explicit WorkSharedFuture(const std::shared_ptr<lenin::WorkFutureImpl<void>> future): 
        future_(future){}
public:
    ~WorkSharedFuture() = default;

    void Get(){
        future_->Get();
    }
};

template<>
class WorkPromise<void>{
private:
    std::shared_ptr<lenin::WorkFutureImpl<void>> future_;
public:
    explicit WorkPromise(){
        future_ = std::make_shared<lenin::WorkFutureImpl<void>>();
    }
    ~WorkPromise() = default;

    void SetValue(){
        future_->UnLock();
    }

    WorkSharedFuture<void> GetSharedFuture(){
        return WorkSharedFuture(future_);
    }
};
}
#endif
