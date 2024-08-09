#ifndef WORK_THREAD_PRIMARY_RUNNABLE_H
#define WORK_THREAD_PRIMARY_RUNNABLE_H
#include <list>
#include "WorkThreadBaseRunnable.h"
#include "WorkThreadPoolDefine.h"
namespace sablin{
namespace lenin{

class WorkThreadPrimaryRunnable: public WorkThreadBaseRunnable{
private:
    std::list<lenin::_WorkRunnable*> cache_queue_;
    const uint32_t id_;
    const uint32_t max_cache_size_;
    std::mutex lock_;
public:
    explicit WorkThreadPrimaryRunnable(
            WorkThreadPool* work_thread_pool,
            uint32_t id,
            uint32_t max_cache_size = kWorkThreadPrimaryCacheSize):
        WorkThreadBaseRunnable(work_thread_pool),
        id_(id),
        max_cache_size_(max_cache_size){}

    ~WorkThreadPrimaryRunnable();

    void Lock() { lock_.lock();}
    bool TryLock() { return lock_.try_lock();}
    void UnLock() { return lock_.unlock();}
    std::mutex& GetLock() { return lock_;}

    uint32_t GetCacheSize(){ return cache_queue_.size();}
    
    bool IsEmpty() const{ return cache_queue_.empty();}

    lenin::_WorkRunnable* Pop(){
        lenin::_WorkRunnable* result = cache_queue_.front();
        cache_queue_.pop_front();
        return result;
    }

    virtual RStatus Run() override;

    virtual RStatus Stop() override;
};

}
}
#endif
