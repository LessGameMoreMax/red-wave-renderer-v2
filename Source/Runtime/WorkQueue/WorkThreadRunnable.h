#ifndef WORK_THREAD_RUNNABLE_H
#define WORK_THREAD_RUNNABLE_H
#include "../Core/HAL/Runnable.h"
#include "WorkQueue.h"
namespace sablin{

class WorkThreadRunnable: public Runnable{
private:
    std::atomic<bool> has_stop_ = false;
public:
    explicit WorkThreadRunnable() = default;
    virtual ~WorkThreadRunnable() = default;

    virtual RStatus Run() override{
        while(true){
            lenin::_WorkRunnable* work = WorkQueue::work_queue_->PopFrontWait(has_stop_);
            if(work == nullptr) break;
            RStatus status;
            status += work->Init();
            if(status.IsOk()) status += work->Run();
            status += work->Destroy();
            delete work;
            ASSERT_WITH_STRING(status.IsOk(), "WorkRunnable Status Is Not Ok!")
        }
        return RStatus();
    }

    virtual RStatus Stop() override{
        WorkQueue::work_queue_->Lock();
        has_stop_ = true;
        WorkQueue::work_queue_->UnLock();

        WorkQueue::work_queue_->NotifyAll();
        return RStatus();
    }

};

}
#endif
