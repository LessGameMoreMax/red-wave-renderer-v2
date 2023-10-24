#ifndef TASK_GRAPH_PERF_ASPECT_H
#define TASK_GRAPH_PERF_ASPECT_H
#include "../../TaskGraphAspect/TaskGraphTemplateAspect.h"
#include "TaskGraphPerfDefine.h"
namespace sablin{

template<typename... Args>
class TaskGraphPerfAspect: public TaskGraphTemplateAspect<Args...>{
private:
    double pipeline_start_ts_;
    double cur_start_ts_;
    TaskGraphPerfInfo* perf_info_;
protected:
    explicit TaskGraphPerfAspect(double start_ts, TaskGraphPerfInfo* perf_info):
        cur_start_ts_(0.0){
        ASSERT_NO_STRING(perf_info != nullptr)
        pipeline_start_ts_ = start_ts;
        perf_info_ = perf_info;
    }

    virtual RStatus BeginRun() final{
        cur_start_ts_ = GetCurrentAccurateMs();
        if(perf_info_->first_start_ts_ == 0){
            perf_info_->first_start_ts_ = cur_start_ts_ - pipeline_start_ts_;
        }
        return RStatus();
    }

    virtual void FinishRun() final{
        auto cur = GetCurrentAccurateMs();
        perf_info_->last_finish_ts_ = cur - pipeline_start_ts_;
        ++perf_info_->loop_;
        perf_info_->accu_cost_ts_ += (cur - cur_start_ts_);
    }
};

}
#endif
