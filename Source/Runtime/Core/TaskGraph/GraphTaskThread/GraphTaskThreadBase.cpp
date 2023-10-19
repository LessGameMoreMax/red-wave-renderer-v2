#include "GraphTaskThreadBase.h"
namespace sablin{

GraphTaskThreadBase::GraphTaskThreadBase():
    is_done_(false),
    is_init_(false),
    is_running_(false),
    graph_task_number_(0),
    graph_task_queue_(nullptr),
    graph_task_priority_queue_(nullptr),
    pool_config_(nullptr),
    thread_(nullptr) {}

GraphTaskThreadBase::~GraphTaskThreadBase(){
    Reset();
    delete thread_;
}

RStatus GraphTaskThreadBase::Destroy(){
    ASSERT_NO_STRING(is_init_);
    Reset();
    return RStatus();
}

GraphTask* GraphTaskThreadBase::PopPoolTask(){
    GraphTask* result = graph_task_queue_->TryPop();
    if(result == nullptr && 
            thread_->GetThreadType() == ThreadType::kThreadTypeGraphTaskSecondaryThread){
        result = graph_task_priority_queue_->TryPop();
    }
    return result;
}

bool GraphTaskThreadBase::PopPoolBatchTask(std::vector<GraphTask*>& tasks){
    bool result = graph_task_queue_->TryPopBatch(tasks, pool_config_->max_pool_batch_size_);
    if(!result &&
            thread_->GetThreadType() == ThreadType::kThreadTypeGraphTaskSecondaryThread){
        result = graph_task_priority_queue_->TryPopBatch(tasks, 1);
    }
    return result;
}

void GraphTaskThreadBase::RunGraphTask(GraphTask* task){
    is_running_ = true;
    (*task)();
    ++graph_task_number_;
    is_running_ = false;
}

void GraphTaskThreadBase::RunGraphTasks(std::vector<GraphTask*>& tasks){
    is_running_ = true;
    for(GraphTask* task: tasks){
        (*task)();
    }
    graph_task_number_ += tasks.size();
    is_running_ = false;

}

void GraphTaskThreadBase::Reset(){
    is_done_ = false;
    ASSERT_NO_STRING(thread_ != nullptr)
    if(thread_->IsJoinable()){
        thread_->Join();
    }
    is_init_ = false;
    is_running_ = false;
    graph_task_number_ = 0;
}

RStatus GraphTaskThreadBase::LoopProcess(){
    if(pool_config_ == nullptr) 
        return RStatus("Pool Config Is NULL", STRING_CODE_LOCATION);

    if(pool_config_->batch_task_enable_){
        while(is_done_) ProcessGraphTasks();
    }else{
        while(is_done_) ProcessGraphTask();
    }

    return RStatus();
}

bool GraphTaskThreadBase::SetThreadPriority(const ThreadPriority thread_priority){
    return thread_->SetThreadPriority(thread_priority);
}

bool GraphTaskThreadBase::SetThreadAffinity(const CpuSet& cpu_set){
    return thread_->SetThreadAffinity(cpu_set);
}

}
