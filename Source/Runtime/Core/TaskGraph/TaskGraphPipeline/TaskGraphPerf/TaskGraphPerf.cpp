#include "TaskGraphPerf.h"
#include "../TaskGraphPipeline.h"
#include "../../../Math/MathTools.h"
#include "TaskGraphPerfAspect.h"
namespace sablin{

RStatus TaskGraphPerf::Perf(TaskGraphPipeline* pipeline){
    ASSERT_NO_STRING(pipeline != nullptr)
    RStatus status;
    status = Inject(pipeline);
    if(!status.IsOk()) return RStatus("TaskGraphPerf::Perf", STRING_CODE_LOCATION);

    status = pipeline->Process();
    if(!status.IsOk()) return RStatus("TaskGraphPerf::Perf", STRING_CODE_LOCATION);

    status = pipeline->Dump();
    if(!status.IsOk()) return RStatus("TaskGraphPerf::Perf", STRING_CODE_LOCATION);

    status = Recover(pipeline);
    if(!status.IsOk()) return RStatus("TaskGraphPerf::Perf", STRING_CODE_LOCATION);

    return status;
}

RStatus TaskGraphPerf::Inject(TaskGraphPipeline* pipeline){
    ASSERT_NO_STRING(pipeline != nullptr)
    const double now = GetCurrentAccurateMs();
    for(auto* cur: pipeline->repository_.elements_){
        cur->perf_info_ = new TaskGraphPerfInfo();
        cur->AddTaskGraphAspect<TaskGraphPerfAspect<double, TaskGraphPerfInfo*>>
            (now, cur->perf_info_);
    }
    return RStatus();
}

RStatus TaskGraphPerf::Recover(TaskGraphPipeline* pipeline){
    ASSERT_NO_STRING(pipeline != nullptr)
    RStatus status;
    for(auto* cur: pipeline->repository_.elements_){
        status += cur->PopLastAspect();
        if(cur->perf_info_ != nullptr){
            delete cur->perf_info_;
            cur->perf_info_ = nullptr;
        }
    }
    return status;
}

}
