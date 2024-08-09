#ifndef WORK_RUNNABLE_H
#define WORK_RUNNABLE_H
#include "Core/Thread/Runnable.h"
#include "WorkFuture.h"
namespace sablin{
namespace lenin{

class _WorkRunnable: public Runnable{
    friend class WorkTimerRunnable;
    friend class WorkThreadBaseRunnable;
protected:
    long times_; // < 0: Run Forever Times!
public:
    explicit _WorkRunnable(long times = 1):
        times_(times){}
    virtual ~_WorkRunnable() = default;
    virtual bool IsTimer() const = 0;
};
}

template<typename R, typename... Args>
class WorkRunnable: public lenin::_WorkRunnable{
    friend class WorkQueue;
private:
    std::tuple<Args...> args_;
    WorkPromise<R> promise_;
public:
    using ReturnType = R;
    template<typename... FwdArgs,
            typename = std::enable_if_t<(std::is_convertible_v<FwdArgs&&, Args>&&...)>>
    explicit WorkRunnable(FwdArgs&&... args):
            args_(std::forward<FwdArgs>(args)...){}
    virtual ~WorkRunnable() = default;

    virtual RStatus Run() override final{
        if(times_ == 1){
            promise_.SetValue(std::apply(
                    [this](Args&&... args){
                        return this->Work(std::forward<Args>(args)...); 
                    }, std::move(args_)));
        }else{
            std::apply([this](Args&&... args){
                        return this->Work(std::forward<Args>(args)...); 
                    }, std::move(args_));
        }
        if(times_ > 0) --times_;
        return RStatus();
    }
    
    virtual bool IsTimer() const final{ return false;}
    virtual R Work(Args... args) = 0;
};

template<typename... Args>
class WorkRunnable<void, Args...>: public lenin::_WorkRunnable{
    friend class WorkQueue;
private:
    std::tuple<Args...> args_;
    WorkPromise<void> promise_;
public:
    using ReturnType = void;
    template<typename... FwdArgs,
            typename = std::enable_if_t<(std::is_convertible_v<FwdArgs&&, Args>&&...)>>
    explicit WorkRunnable(FwdArgs&&... args):
            args_(std::forward<FwdArgs>(args)...){}
    virtual ~WorkRunnable() = default;

    virtual RStatus Run() override final{
        std::apply(
                [this](Args&&... args){
                    this->Work(std::forward<Args>(args)...); 
                }, std::move(args_));
        if(times_ == 1) promise_.SetValue();
        if(times_ > 0) --times_;
        return RStatus();
    }

    virtual bool IsTimer() const final{ return false;}
    virtual void Work(Args... args) = 0;
};
}
#endif
