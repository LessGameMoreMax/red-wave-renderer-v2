#ifndef WORK_RUNNABLE_H
#define WORK_RUNNABLE_H
#include "../Core/HAL/Runnable.h"
#include "WorkResult.h"
namespace sablin{
namespace lenin{

class _WorkRunnable: public Runnable{
public:
    explicit _WorkRunnable() = default;
    virtual ~_WorkRunnable() = default;
};
}

template<typename R, typename... Args>
class WorkRunnable: public lenin::_WorkRunnable{
    friend class WorkQueue;
private:
    std::tuple<Args...> args_;
    WorkPromise<R> promise_;
public:
    template<typename... FwdArgs,
            typename = std::enable_if_t<(std::is_convertible_v<FwdArgs&&, Args>&&...)>>
    explicit WorkRunnable(FwdArgs&&... args):
            args_(std::forward<FwdArgs>(args)...){}
    virtual ~WorkRunnable() = default;

    virtual RStatus Run() override final{
        promise_.SetValue(std::apply(
                [this](Args&&... args){
                    return this->Work(std::forward<Args>(args)...); 
                }, std::move(args_)));
        return RStatus();
    }

    virtual R Work(Args&&... args) = 0;
};

template<typename... Args>
class WorkRunnable<void, Args...>: public lenin::_WorkRunnable{
    friend class WorkQueue;
private:
    std::tuple<Args...> args_;
    WorkPromise<void> promise_;
public:
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
        promise_.SetValue();
        return RStatus();
    }

    virtual void Work(Args&&... args) = 0;
};

}
#endif
