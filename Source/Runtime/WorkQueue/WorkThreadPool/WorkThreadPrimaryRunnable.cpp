#include "WorkThreadPrimaryRunnable.h"
#include "WorkThreadPool.h"
namespace sablin{
namespace lenin{

WorkThreadPrimaryRunnable::~WorkThreadPrimaryRunnable(){
    while(!cache_queue_.empty()){
        delete cache_queue_.front();
        cache_queue_.pop_front();
    }
}

RStatus WorkThreadPrimaryRunnable::Run(){
    while(!has_stop_){
        std::unique_lock<std::mutex> lk(lock_);
        while(IsEmpty() && !has_stop_){
            cache_queue_ = work_thread_pool_->StealWorks(id_, max_cache_size_/2);
            if(cache_queue_.empty()){
                cache_queue_ = work_thread_pool_->PopFrontBatch(id_, max_cache_size_);
            }else{
                std::list<lenin::_WorkRunnable*> temp = 
                    work_thread_pool_->TryPopFrontBatch(id_, max_cache_size_-GetCacheSize());
                cache_queue_.splice(cache_queue_.end(), temp);
            }
        }
        lenin::_WorkRunnable* work = nullptr;
        if(IsEmpty()) continue;
        else work = Pop();
        lk.unlock();

        RunWork(work);
    }
    return RStatus();
}


RStatus WorkThreadPrimaryRunnable::Stop(){
    has_stop_ = true;
    return RStatus();
}

}
}
