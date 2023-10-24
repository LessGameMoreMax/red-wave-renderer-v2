#include "TaskGraphPipelineFactory.h"
namespace sablin{

std::list<TaskGraphPipeline*> TaskGraphPipelineFactory::s_pipeline_list_;
std::mutex TaskGraphPipelineFactory::s_lock_;

TaskGraphPipeline* TaskGraphPipelineFactory::Create(){
    std::lock_guard<std::mutex> lock(s_lock_);
    TaskGraphPipeline* pipeline = new TaskGraphPipeline();
    s_pipeline_list_.emplace_back(pipeline);
    return pipeline;
}

RStatus TaskGraphPipelineFactory::Remove(TaskGraphPipeline* pipeline){
    ASSERT_NO_STRING(pipeline != nullptr)
    std::lock_guard<std::mutex> lock(s_lock_);
    s_pipeline_list_.remove(pipeline);
    if(pipeline != nullptr) delete pipeline;
    return RStatus();
}

RStatus TaskGraphPipelineFactory::Clear(){
    std::lock_guard<std::mutex> lock(s_lock_);
    for(TaskGraphPipeline* pipeline: s_pipeline_list_)
        if(pipeline != nullptr) delete pipeline;
    s_pipeline_list_.clear();
    return RStatus();
}

}
