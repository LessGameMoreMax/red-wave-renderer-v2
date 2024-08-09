#include "WorkThreadMonitorRunnable.h"
#include "WorkThreadPool.h"
#include "WorkThreadSecondaryRunnable.h"
#include "Core/GenericPlatform/GenericPlatformProcess.h"
namespace sablin{
namespace lenin{

WorkThreadMonitorRunnable::WorkThreadMonitorRunnable(WorkThreadPool* work_thread_pool):
    WorkThreadBaseRunnable(work_thread_pool),
    secondary_threads_pool_(work_thread_pool->GetPoolSize()),
    scan_milli_(kWorkThreadMonitorMaxScanMilli){}

WorkThreadMonitorRunnable::~WorkThreadMonitorRunnable(){
    for(uint32_t i = 0;i != work_thread_pool_->GetPoolSize(); ++i)
        ChangeSecondaryThread(i, 0);
}

void WorkThreadMonitorRunnable::EnlargeSecondaryThread(uint32_t id, uint32_t size){
    if(size == 0) return;

    for(uint32_t i = 0; i != size; ++i){
        WorkThreadSecondaryRunnable* runnable = new WorkThreadSecondaryRunnable(work_thread_pool_, i);
        if(runnable == nullptr) break;

        RunnableThread* runnable_thread = PlatformProcess::CreateNativeThread(runnable, "WorkThreadSecondaryThread",
                ThreadPriority::kThreadPriorityNormal, ThreadType::kThreadTypeWorkThreadSecondaryThread);
        if(runnable_thread == nullptr){
            delete runnable;
            break;
        }
        secondary_threads_pool_[id].push_back(runnable_thread);
    }
}

void WorkThreadMonitorRunnable::ReduceSecondaryThread(uint32_t id, uint32_t size){
    if(size == 0) return;

    std::vector<RunnableThread*> threads;
    while(size != 0){
        threads.push_back(secondary_threads_pool_[id].back());
        secondary_threads_pool_[id].pop_back();
        --size;
    }

    for(RunnableThread* thread: threads)
        thread->StopRunnable();

    work_thread_pool_->NotifyWorkPoolAll(id);

    for(RunnableThread* thread: threads){
        thread->Join();
        Runnable* runnable = thread->GetRunnable();
        delete thread;
        delete runnable;
    }
}

RStatus WorkThreadMonitorRunnable::Run(){
    while(!has_stop_){
        std::this_thread::sleep_for(std::chrono::milliseconds(scan_milli_));
        if(has_stop_) break;
        bool busy = false;
        for(uint32_t i = 0;i != work_thread_pool_->GetPoolSize(); ++i){
            uint32_t rough_size = work_thread_pool_->GetWorkPoolRoughSize(i);

            uint32_t thread_size = kWorkThreadSecondaryVeryBusySize;
            if(rough_size == 0) 
                thread_size = 0;
            else if(rough_size < kWorkQueueWorkPoolBusySize)
                thread_size = kWorkThreadSecondaryBusySize;
            ChangeSecondaryThread(i, thread_size);

            busy = thread_size == kWorkThreadSecondaryVeryBusySize;
        }
        if(busy) scan_milli_ = kWorkThreadMonitorMinScanMilli;
        else scan_milli_ = kWorkThreadMonitorMaxScanMilli;
    }
    return RStatus();
}

void WorkThreadMonitorRunnable::ChangeSecondaryThread(uint32_t id, uint32_t size){
    uint32_t thread_size = secondary_threads_pool_[id].size();
    if(size == thread_size) return;
    if(size > thread_size) EnlargeSecondaryThread(id, size - thread_size);
    else ReduceSecondaryThread(id, thread_size - size);
}

}
}
