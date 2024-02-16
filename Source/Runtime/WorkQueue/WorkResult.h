#ifndef WORK_RESULT_H
#define WORK_RESULT_H
#include "../Core/Misc/MacroTools.h"
#include "../Core/HAL/MemoryBase.h"
#include <memory>
namespace sablin{

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

template<typename R>
class WorkSharedFuture{
private:
    std::shared_ptr<WorkFutureImpl<R>> future_;
private:
    explicit WorkSharedFuture(const std::shared_ptr<WorkFutureImpl<R>> future): 
        future_(future){}
public:
    ~WorkSharedFuture() = default;

    R Get(){
        return future_->Get();
    }

    inline void Lock(){
        future_->Lock();
    }

    inline void UnLock(){
        future_->UnLock();
    }
};

template<typename R>
class WorkPromise{
private:
    std::shared_ptr<R> result_;
    std::shared_ptr<WorkFutureImpl<R>> future_;
public:
    explicit WorkPromise(){
        result_ = MemoryBase::BaseMalloc(sizeof(R));
        future_ = std::make_shared<WorkFutureImpl<R>>(result_);
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

template<>
class WorkSharedFuture<void>{
    friend class WorkPromise<void>;
private:
    std::shared_ptr<WorkFutureImpl<void>> future_;
private:
    explicit WorkSharedFuture(const std::shared_ptr<WorkFutureImpl<void>> future): 
        future_(future){}
public:
    ~WorkSharedFuture() = default;

    void Get(){
        future_->Get();
    }

    inline void Lock(){
        future_->Lock();
    }

    inline void UnLock(){
        future_->UnLock();
    }
};

template<>
class WorkPromise<void>{
private:
    std::shared_ptr<WorkFutureImpl<void>> future_;
public:
    explicit WorkPromise(){
        future_ = std::make_shared<WorkFutureImpl<void>>();
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
