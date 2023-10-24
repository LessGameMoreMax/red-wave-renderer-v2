#include "TaskGraphDynamicEngine.h"
namespace sablin{
    
TaskGraphDynamicEngine::TaskGraphDynamicEngine():
    total_end_size_(0),
    finished_end_size_(0),
    run_element_size_(0)
    {}

RStatus TaskGraphDynamicEngine::Setup(const std::set<TaskGraphElement*, TaskGraphElementSorter>& elements){
    total_element_arr_.clear();
    front_element_arr_.clear();
    total_end_size_ = 0;
    for(TaskGraphElement* element: elements){
        ASSERT_NO_STRING(element != nullptr)
        if(element->run_before_.empty()){
            ++total_end_size_;
        }

        if(element->dependence_.empty()){
            front_element_arr_.emplace_back(element);
        }
        total_element_arr_.emplace_back(element);
    }
    return RStatus();
}

RStatus TaskGraphDynamicEngine::Run(){
    RStatus status = BeforeRun();
    if(!status.IsOk()){
        return RStatus("TaskGraphDynamicEngine::Run", STRING_CODE_LOCATION);
    }
    AsyncRun();
    if(!cur_status_.IsOk()){
        status = cur_status_;
    }
    return status;
}

RStatus TaskGraphDynamicEngine::AfterRunCheck(){
    if(run_element_size_ != total_element_arr_.size()){
        return RStatus("Dynamic Engine Run Element Size Not Match...", STRING_CODE_LOCATION);
    }
    for(TaskGraphElement* element: total_element_arr_){
        if(!element->done_){
            return RStatus("Dynamic Engine Run Check Failed...", STRING_CODE_LOCATION);
        }
    }
    return RStatus();
}

void TaskGraphDynamicEngine::AsyncRun(){
    for(const auto& element: front_element_arr_){
        Process(element, element == front_element_arr_.back());
    }
    Wait();
}

RStatus TaskGraphDynamicEngine::BeforeRun(){
    RStatus status;
    finished_end_size_ = run_element_size_ = 0;
    cur_status_ = RStatus();
    for(TaskGraphElement* element: total_element_arr_){
        status += element->BeforeRun();
    }
    return status;
}

RStatus TaskGraphDynamicEngine::Process(TaskGraphElement* element, bool affinity){
    if(cur_status_.IsError())[[unlikely]]{
        return RStatus("Current Status Error!", STRING_CODE_LOCATION);
    }
    const auto& exec = [this, element]{
        auto cur_status = element->FatProcessor(TaskGraphFunctionType::kRun);
        if(cur_status.IsError())[[unlikely]]{
            cur_status_ = cur_status;
        }
        AfterElementRun(element);
    };

    if(affinity && element->GetBindingIndex() == kTaskGraphDefaultBindingIndex){
        exec();
    }else{
        thread_pool_->Commit(exec, CalcIndex(element));
    }
    return RStatus();
}

void TaskGraphDynamicEngine::AfterElementRun(TaskGraphElement* element){
    element->done_ = true;
    ++run_element_size_;

    std::vector<TaskGraphElement*> ready;
    for(auto cur: element->run_before_){
        if(--cur->left_depend_ <= 0){
            ready.emplace_back(cur);
        }
    }

    for(auto& cur: ready){
        Process(cur, cur == ready.back());
    }

    std::unique_lock<std::mutex> lock(lock_);
    if((element->run_before_.empty() && (++finished_end_size_ >= total_end_size_)) ||
            cur_status_.IsError()){
        cv_.notify_one();
    }
}

void TaskGraphDynamicEngine::Wait(){
    std::unique_lock<std::mutex> lock(lock_);
    cv_.wait(lock, [this]{
        return (finished_end_size_ >= total_end_size_) || cur_status_.IsError();
    });
}

}
