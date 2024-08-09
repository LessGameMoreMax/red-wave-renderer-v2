#include "WorkThreadBaseRunnable.h"
#include "WorkThreadPool.h"
#include "Math/Time.h"
#include "Math/MathTools.h"
namespace sablin{
namespace lenin{

void WorkThreadBaseRunnable::RunWork(lenin::_WorkRunnable *work){
    if(work->IsTimer()){
        long start = GetCurrentMs();
        if(!ExcuateWork(work)) return;
        long span = GetCurrentMs() - start;
        WorkTimerRunnable* runnable = dynamic_cast<WorkTimerRunnable*>(work);
        runnable->ttl_ = Max<long>(runnable->expect_ttl_-span, 0);
        work_thread_pool_->PushTimerBack(runnable);
    }else{
        if(ExcuateWork(work))
            work_thread_pool_->PushBack(work);
    }
}

bool WorkThreadBaseRunnable::ExcuateWork(lenin::_WorkRunnable* work){
    RStatus status;
    status += work->Init();
    if(status.IsOk()) status += work->Run();
    status += work->Destroy();

    if(!status.IsOk()) work->times_ = 0;
    if(work->times_ == 0){
        delete work;
        return false;
    }
    return true;
}

}
}
