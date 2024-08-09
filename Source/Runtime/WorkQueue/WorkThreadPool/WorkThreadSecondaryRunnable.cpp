#include "WorkThreadSecondaryRunnable.h"
#include "WorkThreadPool.h"
namespace sablin{
namespace lenin{

RStatus WorkThreadSecondaryRunnable::Run(){
    while(!has_stop_){
        lenin::_WorkRunnable* work = work_thread_pool_->PopFront(id_);
        if(work == nullptr) continue;
        RunWork(work);
    }
    return RStatus();
}

RStatus WorkThreadSecondaryRunnable::Stop(){
    has_stop_ = true;
    return RStatus();
}
}
}
