#ifndef ASYNC_H
#define ASYNC_H
#include <future>
#include "../Debug/Assertion.h"
#include "../HAL/Runnable.h"
#include "../GenericPlatform/GenericPlatformProcess.h"
#include "AsyncFuture.h"
namespace sablin{

template<typename R, typename F, typename... Args>
class AsyncRunnable: public Runnable{
private:
    F func_;
    std::tuple<Args...> args_;
    std::promise<R> promise_;
public:
    template<typename FwdF, typename... FwdArgs,
            typename = std::enable_if_t<(std::is_convertible_v<FwdArgs&&, Args>&&...)>>
    explicit AsyncRunnable(FwdF&& func, FwdArgs&&... args):
        func_(std::forward<FwdF>(func)), args_(std::forward<FwdArgs>(args)...){}

    virtual RStatus Run() override{
        promise_.set_value(std::apply(func_, args_));
        return RStatus();
    }

    std::promise<R>& GetPromise(){
        return promise_;
    }
};

template<typename F, typename... Args>
class AsyncRunnable<void, F, Args...>: public Runnable{
private:
    F func_;
    std::tuple<Args...> args_;
    std::promise<void> promise_;
public:
    template<typename FwdF, typename... FwdArgs,
            typename = std::enable_if_t<(std::is_convertible_v<FwdArgs&&, Args>&&...)>>
    explicit AsyncRunnable(FwdF&& func, FwdArgs&&... args):
        func_(std::forward<FwdF>(func)), args_(std::forward<FwdArgs>(args)...){}

    virtual RStatus Run() override{
        std::apply(func_, args_);
        promise_.set_value();
        return RStatus();
    }

    std::promise<void>& GetPromise(){
        return promise_;
    }
};

template<typename F, typename... Args>
auto MakeAsyncRunnable(F&& f, Args&&... args){
    return new AsyncRunnable<typename std::result_of<F(Args...)>::type, std::decay_t<F>,
           std::remove_cv_t<Args>...>(std::forward<F>(f), std::forward<Args>(args)...);
}

template<typename F, typename... Args,
    typename R = typename std::result_of<F(Args...)>::type>
static AsyncFuture<R> AsyncLanuchAsync(F&& fn, Args&&... args){
    auto runnable = MakeAsyncRunnable(std::forward<F>(fn), std::forward<Args>(args)...);
    ASSERT_NO_STRING(runnable != nullptr)
    RunnableThread* runnable_thread = PlatformProcess::CreateNativeThread(runnable, "AsyncLaunchAsyncThread",
            ThreadPriority::kThreadPriorityNormal,
            ThreadType::kThreadTypeAsyncLaunchAsync);
    return std::move(AsyncFuture<R>(std::move(runnable->GetPromise().get_future()), runnable, runnable_thread));
}
}
#endif
