#include "TaskGraphElementRepository.h"
namespace sablin{

TaskGraphElementRepository::TaskGraphElementRepository():
    cur_state_(TaskGraphElementState::kCreate){}

TaskGraphElementRepository::~TaskGraphElementRepository(){
    for(TaskGraphElement* element: elements_)
        if(element != nullptr)
            delete element;
    elements_.clear();
}

void TaskGraphElementRepository::Insert(TaskGraphElement* ptr){
    ASSERT_NO_STRING(ptr != nullptr)
    elements_.insert(ptr);
}

bool TaskGraphElementRepository::Find(TaskGraphElement* ptr) const{
    ASSERT_NO_STRING(ptr != nullptr)
    return elements_.find(ptr) != elements_.end();
}

TaskGraphElementRepository* TaskGraphElementRepository::SetThreadPool(GraphTaskThreadPool* ptr){
    ASSERT_NO_STRING(ptr != nullptr)
    for(auto& cur: elements_)
        cur->SetThreadPool(ptr);
    return this;
}

RStatus TaskGraphElementRepository::PreSetup(){
    return PullAllState(TaskGraphElementState::kNormal);
}

RStatus TaskGraphElementRepository::Reset(){
    RStatus status;
    for(auto& cur: async_elements_){
        if(cur->timeout_strategy_ == TaskGraphElementTimeoutStrategy::kHoldByPipeline){
            status += cur->GetAsyncResult();
        }
        if(cur->IsTimeout()){
            cur->cur_state_.store(TaskGraphElementState::kNormal);
        }
    }
    return status;
}

RStatus TaskGraphElementRepository::PullAllState(const TaskGraphElementState& state){
    RStatus status;
    if(cur_state_ == state) return status;
    cur_state_ = state;
    for(auto& cur: elements_){
        cur->cur_state_.store(state);
        if(state != TaskGraphElementState::kYield)
            cur->yield_cv_.notify_one();
    }
    return status;
}

bool TaskGraphElementRepository::IsCancelState() const{
    return cur_state_ == TaskGraphElementState::kCancel;
}

RStatus TaskGraphElementRepository::Setup(){
    RStatus status;
    async_elements_.clear();
    for(auto& element: elements_){
        ASSERT_NO_STRING(element != nullptr)
        status = element->CheckSuitable();
        if(status.IsOk()) return RStatus("TaskGraphElementRepository::Setup", STRING_CODE_LOCATION);
        if(element->IsAsync()){
            async_elements_.emplace(element);
        }
    }
    status = PullAllState(TaskGraphElementState::kNormal);
    return status;
}

RStatus TaskGraphElementRepository::Exit(){
    return PullAllState(TaskGraphElementState::kCreate);
}


}
