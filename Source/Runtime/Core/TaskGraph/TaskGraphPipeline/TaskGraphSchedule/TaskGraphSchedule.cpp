#include "TaskGraphSchedule.h"
namespace sablin{

TaskGraphSchedule::TaskGraphSchedule():
    unique_thread_pool_(nullptr),
    shared_thread_pool_(nullptr),
    schedule_type_(TaskGraphScheduleType::kTaskGraphScheduleTypeUnique){}

TaskGraphSchedule::~TaskGraphSchedule(){}

RStatus TaskGraphSchedule::MakeType(GraphTaskThreadPool* thread_pool){
    delete unique_thread_pool_;
    unique_thread_pool_ = nullptr;

    shared_thread_pool_ = thread_pool;

    schedule_type_ = thread_pool == nullptr? TaskGraphScheduleType::kTaskGraphScheduleTypeUnique: 
            TaskGraphScheduleType::kTaskGraphScheduleTypeShared;
    return RStatus();
}

GraphTaskThreadPool* TaskGraphSchedule::GetThreadPool(){
    return unique_thread_pool_ == nullptr? shared_thread_pool_ : unique_thread_pool_;
}

RStatus TaskGraphSchedule::Setup(){
    if(schedule_type_ == TaskGraphScheduleType::kTaskGraphScheduleTypeUnique){
        if(unique_thread_pool_ != nullptr) delete unique_thread_pool_;
        unique_thread_pool_ = new GraphTaskThreadPool(new GraphTaskThreadPoolConfig(pool_config_));
        unique_thread_pool_->Setup();
    }else if(schedule_type_ == TaskGraphScheduleType::kTaskGraphScheduleTypeShared){
        ASSERT_NO_STRING(shared_thread_pool_ != nullptr)
        ASSERT_NO_STRING(shared_thread_pool_->IsInit())
    }
    return RStatus();
}

RStatus TaskGraphSchedule::Exit(){
    if(schedule_type_ == TaskGraphScheduleType::kTaskGraphScheduleTypeUnique){
        delete unique_thread_pool_;
    }
    return RStatus();
}

}
