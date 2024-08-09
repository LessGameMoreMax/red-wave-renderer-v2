#ifndef WORK_THREAD_SECONDARY_RUNNABLE_H
#define WORK_THREAD_SECONDARY_RUNNABLE_H
#include "WorkThreadBaseRunnable.h"
namespace sablin{
namespace lenin{

class WorkThreadSecondaryRunnable: public WorkThreadBaseRunnable{
private:
    const uint32_t id_;
public:
    explicit WorkThreadSecondaryRunnable(
            WorkThreadPool* work_thread_pool,
            uint32_t id):
        WorkThreadBaseRunnable(work_thread_pool),
        id_(id){}
    ~WorkThreadSecondaryRunnable() = default;
    
    virtual RStatus Run() override;

    virtual RStatus Stop() override;
};

}

}
#endif
