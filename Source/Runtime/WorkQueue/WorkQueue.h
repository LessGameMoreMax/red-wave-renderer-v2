#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H
#include "../Core/Misc/MacroTools.h"
#include "../Container/ProducerConsumerQueue.h"
#include "WorkFunctionRunnable.h"
#include "WorkThreadPool.h"
#include "WorkResult.h"
namespace sablin{

class WorkQueue final{
    friend class WorkThreadRunnable;
private:
    inline static ProducerConsumerQueue<lenin::_WorkRunnable*>* work_queue_ = nullptr;
    inline static WorkThreadPool* work_thread_pool_ = nullptr;
private:
    explicit WorkQueue() = default;
public:
    CLASS_NO_ALLOWED_COPY(WorkQueue)
    ~WorkQueue();

    static void Initialize();
    static void Exit(bool should_wait = true);

    template<typename W, typename... Args>
    requires std::is_base_of_v<WorkRunnable<typename W::ReturnType, std::remove_cv_t<Args>...>, W>
    static WorkSharedFuture<typename W::ReturnType> CommitWork(Args&&... args){
        WorkRunnable<typename W::ReturnType, std::remove_cv_t<Args>...>* work_runnable =
                new W(std::forward<Args>(args)...);
        ASSERT_NO_STRING(work_runnable != nullptr)
        auto work_shared_future = work_runnable->promise_.GetSharedFuture();
        work_queue_->PushBack(static_cast<lenin::_WorkRunnable*>(work_runnable));
        return work_shared_future;
    }

    template<typename F, typename... Args,
        typename R = typename std::result_of<F(Args...)>::type>
    static WorkSharedFuture<R> CommitWork(F&& fn, Args&&... args){
        auto work_runnable = MakeWorkFunctionRunnable(std::forward<F>(fn), std::forward<Args>(args)...);
        ASSERT_NO_STRING(work_runnable != nullptr)
        auto work_shared_future = work_runnable->promise_.GetSharedFuture();
        work_queue_->PushBack(static_cast<lenin::_WorkRunnable*>(work_runnable));
        return work_shared_future;
    }

};

}
#endif
