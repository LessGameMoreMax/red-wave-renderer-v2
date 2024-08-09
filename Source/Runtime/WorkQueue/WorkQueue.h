#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H
#include "Macro/MacroTools.h"
#include "Container/ProducerConsumerQueue.h"
#include "WorkFunctionRunnable.h"
#include "WorkThreadPool/WorkThreadPool.h"
#include "WorkFuture.h"
namespace sablin{

class WorkQueue final{
private:
    inline static WorkThreadPool* work_thread_pool_ = nullptr;
private:
    explicit WorkQueue() = default;
public:
    CLASS_NO_ALLOWED_COPY(WorkQueue)
    ~WorkQueue() = default;

    static void Initialize();
    static void Exit();

    template<typename W, typename... Args>
    requires std::is_base_of_v<WorkRunnable<typename W::ReturnType, std::remove_cv_t<Args>...>, W>
    static WorkSharedFuture<typename W::ReturnType> CommitWork(Args&&... args){
        WorkRunnable<typename W::ReturnType, std::remove_cv_t<Args>...>* work_runnable =
                new W(std::forward<Args>(args)...);
        ASSERT_NO_STRING(work_runnable != nullptr)
        auto work_shared_future = work_runnable->promise_.GetSharedFuture();
        work_thread_pool_->PushBack(static_cast<lenin::_WorkRunnable*>(work_runnable));
        return work_shared_future;
    }

    template<typename F, typename... Args,
        typename R = typename std::result_of<F(Args...)>::type>
    static WorkSharedFuture<R> CommitWork(F&& fn, Args&&... args){
        auto work_runnable = lenin::MakeWorkFunctionRunnable(std::forward<F>(fn), std::forward<Args>(args)...);
        ASSERT_NO_STRING(work_runnable != nullptr)
        auto work_shared_future = work_runnable->promise_.GetSharedFuture();
        work_thread_pool_->PushBack(static_cast<lenin::_WorkRunnable*>(work_runnable));
        return work_shared_future;
    }

    template<typename W, typename... Args>
    requires std::is_base_of_v<WorkRunnable<typename W::ReturnType, std::remove_cv_t<Args>...>, W>
    static WorkSharedFuture<typename W::ReturnType> CommitTimerWork(long times, long ttl, Args&&... args){
        WorkRunnable<typename W::ReturnType, std::remove_cv_t<Args>...>* work_runnable =
                new W(std::forward<Args>(args)...);
        ASSERT_NO_STRING(work_runnable != nullptr)
        auto work_shared_future = work_runnable->promise_.GetSharedFuture();
        lenin::WorkTimerRunnable* work_timer_runnable = new lenin::WorkTimerRunnable(work_runnable, times, ttl);
        ASSERT_NO_STRING(work_timer_runnable != nullptr)
        work_thread_pool_->PushTimerBack(work_timer_runnable);
        return work_shared_future;
    }

    template<typename F, typename... Args,
        typename R = typename std::result_of<F(Args...)>::type>
    static WorkSharedFuture<R> CommitTimerWork(long times, long ttl, F&& fn, Args&&... args){
        auto work_runnable = lenin::MakeWorkFunctionRunnable(std::forward<F>(fn), std::forward<Args>(args)...);
        ASSERT_NO_STRING(work_runnable != nullptr)
        auto work_shared_future = work_runnable->promise_.GetSharedFuture();
        lenin::WorkTimerRunnable* work_timer_runnable = new lenin::WorkTimerRunnable(work_runnable, times, ttl);
        ASSERT_NO_STRING(work_timer_runnable != nullptr)
        work_thread_pool_->PushTimerBack(work_timer_runnable);
        return work_shared_future;
    }

};

}
#endif
