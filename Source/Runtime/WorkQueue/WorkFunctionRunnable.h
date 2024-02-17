#ifndef WORK_FUNCTION_RUNNABLE_H
#define WORK_FUNCTION_RUNNABLE_H
#include "WorkRunnable.h"
namespace sablin{

template<typename R, typename F, typename... Args>
class WorkFunctionRunnable final: public WorkRunnable<R, Args...>{
private:
    F func_;
public:
    template<typename FwdF, typename... FwdArgs,
        typename = std::enable_if_t<(std::is_convertible_v<FwdArgs&&, Args>&&...)>>
    explicit WorkFunctionRunnable(FwdF&& func, FwdArgs&&... args):
        WorkRunnable<R, Args...>(std::forward<FwdArgs>(args)...),
        func_(std::forward<FwdF>(func)){}

    virtual ~WorkFunctionRunnable() = default;

    virtual R Work(Args&&... args) override{
        return func_(std::forward<Args>(args)...);
    }
};

template<typename F, typename... Args,
    typename R= typename std::result_of<F(Args...)>::type>
auto MakeWorkFunctionRunnable(F&& f, Args&&... args) -> WorkRunnable<R, std::remove_cv_t<Args>...>*{
    return new WorkFunctionRunnable<R, std::decay_t<F>, std::remove_cv_t<Args>...>(std::forward<F>(f), std::forward<Args>(args)...);
}

}
#endif
