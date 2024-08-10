#ifndef WORK_THREAD_POOL_H
#define WORK_THREAD_POOL_H
#include <list>
#include <mutex>
#include <vector>
#include "WorkQueue/WorkRunnable.h"
#include "Core/Thread/RunnableThread.h"
#include "WorkThreadPoolDefine.h"
#include "Container/ProducerConsumerQueue.h"
#include "WorkThreadPrimaryRunnable.h"
#include "WorkQueue/WorkTimerRunnable.h"
#include "WorkThreadTimerRunnable.h"
namespace sablin{

class WorkThreadPool{
private:
    std::vector<RunnableThread*> primary_work_threads_;
    std::vector<lenin::WorkThreadPrimaryRunnable*> primary_runnable_pool_;

    std::vector<ProducerConsumerQueue<lenin::_WorkRunnable*>*> work_pool_;
    std::atomic<uint32_t> index_;

    RunnableThread* timer_thread_;
    lenin::WorkThreadTimerRunnable* timer_runnable_;

    RunnableThread* monitor_thread_;

    std::atomic<bool> has_init_;
    std::atomic<bool> has_stop_;
private:
    void Initialize();
    void Exit();
    uint32_t CreateWorkPool(const uint32_t pool_size);
    uint32_t CreatePrimaryThreads(const uint32_t thread_num);
    void CreateTimerThread();
    void CreateMonitorThread();
    void ExitMonitorThread();
    void ExitTimerThread();
    void ExitPrimaryThreads();
    void ExitWorkPool();
public:
    explicit WorkThreadPool();
    CLASS_NO_ALLOWED_COPY(WorkThreadPool)
    ~WorkThreadPool();

    uint32_t GetPoolSize(){ return work_pool_.size();}
    uint32_t GetWorkPoolRoughSize(uint32_t id) { return work_pool_[id]->GetRoughSize();}
    void Stop(){ has_stop_ = true;}

    std::list<lenin::_WorkRunnable*> StealWorks(uint32_t id, uint32_t max_num);
    lenin::_WorkRunnable* PopFront(uint32_t id);
    std::list<lenin::_WorkRunnable*> PopFrontBatch(uint32_t id, uint32_t max_num);
    lenin::_WorkRunnable* TryPopFront(uint32_t id);
    std::list<lenin::_WorkRunnable*> TryPopFrontBatch(uint32_t id, uint32_t max_num);

    void PushBack(lenin::_WorkRunnable* runnable);
    void PushTimerBack(lenin::WorkTimerRunnable* runnable);
};

}
#endif
