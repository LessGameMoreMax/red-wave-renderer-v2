#ifndef WORK_THREAD_BASE_RUNNABLE_H
#define WORK_THREAD_BASE_RUNNABLE_H
#include "Core/Thread/Runnable.h"
#include "Container/ProducerConsumerQueue.h"
#include "WorkQueue/WorkRunnable.h"
#include <atomic>
namespace sablin{

class WorkThreadPool;

namespace lenin{

class WorkThreadBaseRunnable: public Runnable{
protected:
    std::atomic<bool> has_stop_;
    WorkThreadPool* work_thread_pool_;
public:
    explicit WorkThreadBaseRunnable(WorkThreadPool* work_thread_pool):
            has_stop_(false),
            work_thread_pool_(work_thread_pool){}
    virtual ~WorkThreadBaseRunnable() = default;
    bool HasStop() { return has_stop_;}

    void RunWork(lenin::_WorkRunnable* work);
    bool ExcuateWork(lenin::_WorkRunnable* work);
};

}
}
#endif
