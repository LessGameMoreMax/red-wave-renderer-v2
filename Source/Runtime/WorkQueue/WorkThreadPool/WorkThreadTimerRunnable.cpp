#include "WorkThreadTimerRunnable.h"
#include "WorkThreadPool.h"
namespace sablin{
namespace lenin{

WorkThreadTimerRunnable::~WorkThreadTimerRunnable(){
    while(!priority_queue_.IsEmpty()){
        delete priority_queue_.Top().first;
        priority_queue_.Pop();
    }
}

RStatus WorkThreadTimerRunnable::Run(){
    while(!has_stop_){
        while(!priority_queue_.IsEmpty() && 
                priority_queue_.Top().second <= std::chrono::steady_clock::now()){
            lenin::WorkTimerRunnable* runnable = priority_queue_.Top().first;
            priority_queue_.Pop();
            if(runnable->work_timer_info_->HasStop()) delete runnable;
            else work_thread_pool_->PushBack(runnable);
        }
        lenin::WorkTimerRunnable* runnable = nullptr;
        if(priority_queue_.IsEmpty()) 
            runnable = timer_queue_.PopFront();
        else 
            runnable = timer_queue_.PopFrontUntil(priority_queue_.Top().second);
        if(runnable == nullptr) continue;
        priority_queue_.Push(runnable, std::chrono::time_point_cast<std::chrono::milliseconds>
                (std::chrono::steady_clock::now()) + std::chrono::milliseconds(runnable->ttl_));
    }
    return RStatus();
}

void WorkThreadTimerRunnable::PushTimerBack(lenin::WorkTimerRunnable *runnable){
    timer_queue_.PushBack(runnable);
}

}
}
