#ifndef ASYNC_H
#define ASYNC_H
#include <future>
#include "../Debug/Assertion.h"
#include "../HAL/Runnable.h"
#include "../GenericPlatform/GenericPlatformProcess.h"

namespace sablin{

template<typename F, typename... Args>
class AsyncRunnable: public Runnable{
    using R = typename std::result_of<F(Args...)>::type;
private:
    std::function<R(Args...)> func_;
    std::tuple<Args...> args_;
    std::promise<R> promise_;
public:
    explicit AsyncRunnable(F&& func, Args&&... args):
        func_(std::forward<F>(func)), args_(std::forward<Args>(args)...){}

    virtual RStatus Run() override{
        promise_.set_value(std::apply(func_, args_));
    }

    const std::promise<R>& GetPromise() const{
        return promise_;
    }
};

template<typename F, typename... Args,
    typename R = typename std::result_of<F(Args...)>::type>
static std::future<R> AsyncLanuchAsync(F&& fn, Args&&... args){
    AsyncRunnable<std::decay_t<F>, std::remove_cv_t<std::remove_reference_t<Args>>...>* 
        runnable = new AsyncRunnable(fn, args...);
    ASSERT_NO_STRING(runnable != nullptr)
    PlatformProcess::CreateNativeThread(runnable, "AsyncLaunchAsyncThread",
            ThreadPriority::kThreadPriorityNormal,
            ThreadType::kThreadTypeAsyncLaunchAsync);
    return runnable->GetPromise().get_future();
}
}
#endif
