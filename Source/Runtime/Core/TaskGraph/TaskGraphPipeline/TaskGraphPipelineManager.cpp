#include "TaskGraphPipelineManager.h"
namespace sablin{

TaskGraphPipelineManager::TaskGraphPipelineManager():
    is_init_(false){}

TaskGraphPipelineManager::~TaskGraphPipelineManager(){
    Clear();
}

RStatus TaskGraphPipelineManager::Init(){
    ASSERT_NO_STRING(used_list_.empty())
    RStatus status;
    for(auto* pipeline: free_list_)
        status += pipeline->Init();
    ASSERT_NO_STRING(status.IsOk())
    is_init_ = true;
    return status;
}

RStatus TaskGraphPipelineManager::Run(){
    ASSERT_NO_STRING(is_init_ == true)
    TaskGraphPipeline* pipeline = Fetch();
    if(pipeline == nullptr) return RStatus("No Free Pipeline Current!", STRING_CODE_LOCATION);
    RStatus status = pipeline->Run();
    status += Release(pipeline);
    return status;
}

RStatus TaskGraphPipelineManager::Destroy(){
    ASSERT_NO_STRING(used_list_.empty())
    RStatus status;
    for(auto* pipeline: free_list_){
        status += pipeline->Destroy();
    }
    ASSERT_NO_STRING(status.IsOk())
    is_init_ = false;
    return status;
}

RStatus TaskGraphPipelineManager::Add(TaskGraphPipeline* ptr){
    ASSERT_NO_STRING(ptr != nullptr)
    ASSERT_NO_STRING(is_init_ == false)
    free_list_.push_back(ptr);
    return RStatus();
}

RStatus TaskGraphPipelineManager::Clear(){
    ASSERT_NO_STRING(is_init_ == false)
    used_list_.clear();
    free_list_.clear();
    return RStatus();
}

bool TaskGraphPipelineManager::Find(TaskGraphPipeline* ptr) const{
    bool result = std::any_of(used_list_.begin(), used_list_.end(),
            [ptr](TaskGraphPipeline* cur){return ptr == cur; });
    result |= std::any_of(free_list_.begin(), free_list_.end(),
            [ptr](TaskGraphPipeline* cur){return ptr == cur;});
    return result;
}

RStatus TaskGraphPipelineManager::Remove(TaskGraphPipeline* ptr){
    ASSERT_NO_STRING(is_init_ == false)
    ASSERT_NO_STRING(ptr != nullptr)
    free_list_.remove(ptr);
    return RStatus();
}

size_t TaskGraphPipelineManager::GetSize() const{
    return free_list_.size() + used_list_.size();
}

TaskGraphPipeline* TaskGraphPipelineManager::Fetch(){
    TaskGraphPipeline* pipeline = nullptr;
    std::lock_guard<std::mutex> lk(lock_);
    if(free_list_.empty()) return pipeline;
    pipeline = free_list_.front();
    free_list_.pop_front();
    used_list_.push_back(pipeline);
    return pipeline;
}

RStatus TaskGraphPipelineManager::Release(TaskGraphPipeline* ptr){
    ASSERT_NO_STRING(ptr != nullptr)
    std::lock_guard<std::mutex> lk(lock_);
    bool result = std::any_of(used_list_.begin(), used_list_.end(),
            [ptr](TaskGraphPipeline* cur){return cur == ptr;});
    if(!result){
        return RStatus("Pipeline Is Not Used!", STRING_CODE_LOCATION);
    }
    used_list_.remove(ptr);
    free_list_.push_front(ptr);
    return RStatus();
}


    
}
