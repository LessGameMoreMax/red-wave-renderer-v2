#ifndef WORK_THREAD_TIMER_RUNNABLE_H
#define WORK_THREAD_TIMER_RUNNABLE_H
#include "Container/ProducerConsumerQueue.h"
#include "Container/PriorityQueue.h"
#include "WorkQueue/WorkTimerRunnable.h"
#include "WorkThreadBaseRunnable.h"
#include <atomic>
#include <chrono>
namespace sablin{
namespace lenin{

class WorkThreadTimerRunnable final: public lenin::WorkThreadBaseRunnable{
private:
    ProducerConsumerQueue<lenin::WorkTimerRunnable*> timer_queue_;
    PriorityQueue<lenin::WorkTimerRunnable*, 
        std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds>> priority_queue_;
public:
    explicit WorkThreadTimerRunnable(WorkThreadPool* work_thread_pool):
        WorkThreadBaseRunnable(work_thread_pool){}
    virtual ~WorkThreadTimerRunnable();

    virtual RStatus Run() override;

    virtual RStatus Stop() override { 
        timer_queue_.Stop(true);
        has_stop_ = true;
        return RStatus();
    }

    void PushTimerBack(lenin::WorkTimerRunnable* runnable);
    void NotifyTimerThread() {timer_queue_.NotifyOne(); }
};

}
}
#endif
