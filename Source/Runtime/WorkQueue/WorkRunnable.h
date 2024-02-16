#ifndef WORK_RUNNABLE_H
#define WORK_RUNNABLE_H
#include "../Core/HAL/Runnable.h"
#include <future>
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
private:
    std::tuple<Args...> args_;
    std::promise<R> promise_;
public:
    template<typename... FwdArgs,
            typename = std::enable_if_t<(std::is_convertible_v<FwdArgs&&, Args>&&...)>>
    explicit WorkRunnable(FwdArgs&&... args):
            args_(std::forward<FwdArgs>(args)...){}
    virtual ~WorkRunnable() = default;

    virtual RStatus Run() override final{
        promise_.set_value(std::apply(
                [=](Args&&... args){
                    return this->Work(std::forward<Args>(args)...); 
                }, args_));
        return RStatus();
    }

    virtual R Work(Args&&... args) = 0;
};

template<typename... Args>
class WorkRunnable<void, Args...>: public lenin::_WorkRunnable{
private:
    std::tuple<Args...> args_;
    std::promise<void> promise_;
public:
    template<typename... FwdArgs,
            typename = std::enable_if_t<(std::is_convertible_v<FwdArgs&&, Args>&&...)>>
    explicit WorkRunnable(FwdArgs&&... args):
            args_(std::forward<FwdArgs>(args)...){}
    virtual ~WorkRunnable() = default;

    virtual RStatus Run() override final{
        std::apply(
                [=](Args&&... args){
                    this->Work(std::forward<Args>(args)...); 
                }, args_);
        promise_.set_value();
        return RStatus();
    }

    virtual void Work(Args&&... args) = 0;
};

}
#endif
