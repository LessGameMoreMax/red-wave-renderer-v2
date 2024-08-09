#ifndef WORK_THREAD_MONITOR_RUNNABLE_H
#define WORK_THREAD_MONITOR_RUNNABLE_H
#include "WorkThreadBaseRunnable.h"
#include "WorkThreadPoolDefine.h"
#include "Core/Thread/RunnableThread.h"
#include <vector>
namespace sablin{
namespace lenin{

class WorkThreadMonitorRunnable final: public lenin::WorkThreadBaseRunnable{
private:
    std::vector<std::vector<RunnableThread*>> secondary_threads_pool_;
    uint32_t scan_milli_;
private:
    void EnlargeSecondaryThread(uint32_t id, uint32_t size);
    void ReduceSecondaryThread(uint32_t id, uint32_t size);
public:
    explicit WorkThreadMonitorRunnable(WorkThreadPool* work_thread_pool);
    virtual ~WorkThreadMonitorRunnable();

    virtual RStatus Run() override;

    virtual RStatus Stop() override{
        has_stop_ = true;
        return RStatus();
    }

    void ChangeSecondaryThread(uint32_t id, uint32_t size);
};

}
}
#endif
