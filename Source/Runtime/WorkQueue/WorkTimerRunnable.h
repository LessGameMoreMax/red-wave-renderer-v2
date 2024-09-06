#ifndef WORK_TIMER_RUNNABLE_H
#define WORK_TIMER_RUNNABLE_H
#include "WorkRunnable.h"
#include <chrono>
#include <memory>
#include <atomic>
namespace sablin{

class WorkTimerInfo{
private:
    std::atomic<bool> has_stop_;
public:
    explicit WorkTimerInfo(): has_stop_(false){}

    void Stop(){
        has_stop_ = true;
    }

    bool HasStop() const{
        return has_stop_;
    }
};

namespace lenin{

class WorkTimerInterface{
    friend class WorkThreadTimerRunnable;
    friend class WorkThreadBaseRunnable;
protected:
    unsigned long ttl_;
    const unsigned long expect_ttl_;
public:
    explicit WorkTimerInterface(unsigned long ttl = 0):
        ttl_(ttl),
        expect_ttl_(ttl){}
};

class WorkTimerRunnable final: public _WorkRunnable,
                               public WorkTimerInterface{
private:
    _WorkRunnable* work_runnable_;
    bool has_init_;
public:
    std::shared_ptr<WorkTimerInfo> work_timer_info_;

    WorkTimerRunnable(_WorkRunnable* work_runnable,
            long times, unsigned long ttl):
            _WorkRunnable(times),
            WorkTimerInterface(ttl),
            work_runnable_(work_runnable),
            has_init_(false),
            work_timer_info_(std::make_shared<WorkTimerInfo>()){
            work_runnable_->times_ = times;
        }
    virtual ~WorkTimerRunnable(){
        delete work_runnable_;
    }

    virtual RStatus Init() override{
        if(has_init_) return RStatus();
        has_init_ = true;
        return work_runnable_->Init();
    }
    
    virtual RStatus Run() override{
        RStatus status = work_runnable_->Run();
        times_ = work_runnable_->times_;
        return status;
    }

    virtual RStatus Destroy() override{
        if(times_ == 0) return work_runnable_->Destroy();
        return RStatus();
    }

    virtual bool IsTimer() const final{ return true;}
};

}
}
#endif
