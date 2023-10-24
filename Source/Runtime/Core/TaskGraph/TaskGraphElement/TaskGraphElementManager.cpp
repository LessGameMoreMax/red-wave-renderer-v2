#include "TaskGraphElementManager.h"
#include "TaskGraphOptimizer/TaskGraphMaxParaOptimizer.h"
namespace sablin{

TaskGraphElementManager::TaskGraphElementManager():
    engine_(nullptr),
    engine_type_(TaskGraphEngineType::kDynamic),
    thread_pool_(nullptr),
    auto_check_enable_(true){}

TaskGraphElementManager::~TaskGraphElementManager(){
    if(engine_ != nullptr){
        delete engine_;
        engine_ = nullptr;
    }
}

RStatus TaskGraphElementManager::Init(){
    RStatus status;
    for(auto& element: manager_elements_){
        ASSERT_NO_STRING(element != nullptr)
    }
    status = InitEngine();
    if(!status.IsOk()){
        return RStatus("TaskGraphElementManager::Init", STRING_CODE_LOCATION);
    }
    for(TaskGraphElement* element: manager_elements_){
        status = element->FatProcessor(TaskGraphFunctionType::kInit);
        if(!status.IsOk()){
            return RStatus("TaskGraphElementManager::Init", STRING_CODE_LOCATION);
        }
        element->is_init_ = true;
        element->trigger_times_ = 0;
    }
    return status;
}

RStatus TaskGraphElementManager::Run(){
    RStatus status;
    ASSERT_NO_STRING(engine_ != nullptr)
    status = engine_->Run();
    if(!status.IsOk()){
        return RStatus("TaskGraphElementManager::Run", STRING_CODE_LOCATION);
    }
    if(auto_check_enable_){
        status = engine_->AfterRunCheck();
    }
    return status;
}

RStatus TaskGraphElementManager::Destroy(){
    RStatus status;
    for(TaskGraphElement* element: manager_elements_){
        status = element->FatProcessor(TaskGraphFunctionType::kDestroy);
        if(!status.IsOk()){
            return RStatus("TaskGraphElementManager::Destroy", STRING_CODE_LOCATION);
        }
        element->is_init_ = false;
        element->trigger_times_ = 0;
    }
    if(engine_ != nullptr){
        delete engine_;
        engine_ = nullptr;
    }
    return status;
}

RStatus TaskGraphElementManager::Add(TaskGraphElement* element){
    ASSERT_NO_STRING(element != nullptr)
    manager_elements_.emplace(element);
    return RStatus();
}

RStatus TaskGraphElementManager::Remove(TaskGraphElement* element){
    ASSERT_NO_STRING(element != nullptr)
    manager_elements_.erase(element);
    return RStatus();
}

bool TaskGraphElementManager::Find(TaskGraphElement* element) const{
    if(element == nullptr) return false;
    return manager_elements_.end() != manager_elements_.find(element);
}

RStatus TaskGraphElementManager::Clear(){
    for(auto element: manager_elements_)
        if(element != nullptr)
            delete element;
    manager_elements_.clear();
    return RStatus();
}

TaskGraphElementManager* TaskGraphElementManager::SetScheduleStrategy(int32_t strategy){
    ASSERT_NO_STRING(engine_ != nullptr) 
    engine_->schedule_strategy_ = strategy;
    return this;
}

TaskGraphElementManager* TaskGraphElementManager::SetEngineType(TaskGraphEngineType engine_type){
    engine_type_ = engine_type;
    return this;
}

RStatus TaskGraphElementManager::InitEngine(){
    if(engine_ != nullptr){
        delete engine_;
        engine_ = nullptr;
    }
    switch(engine_type_){
        case TaskGraphEngineType::kStatic: engine_ = new TaskGraphStaticEngine(); break;
        case TaskGraphEngineType::kDynamic: engine_ = new TaskGraphDynamicEngine(); break;
        default: return RStatus("Unknown Engine Type!", STRING_CODE_LOCATION);
    }
    engine_->thread_pool_ = thread_pool_;
    return engine_->Setup(manager_elements_);
}

TaskGraphElementManager* TaskGraphElementManager::SetThreadPool(GraphTaskThreadPool* ptr){
    ASSERT_NO_STRING(ptr != nullptr)
    thread_pool_ = ptr;
    return this;
}

RStatus TaskGraphElementManager::CalcMaxParaSize(size_t& size){
    RStatus status;
    TaskGraphMaxParaOptimizer op;
    if(op.Match(manager_elements_)){
        size = op.GetMaxParaSize(manager_elements_);
    }else{
        size = 0;
        status = RStatus("Can Not Calculate Max Parallel Size Within Groups!", STRING_CODE_LOCATION);
    }
    return status;
}

bool TaskGraphElementManager::CheckSerializable(){
    if(engine_type_ != TaskGraphEngineType::kDynamic) return false;
    int32_t front_size = 0;
    int32_t tail_size = 0;
    for(auto& cur: manager_elements_){
        if(!cur->IsSerializable() ||
                cur->run_before_.size() > 1||
                cur->dependence_.size() > 1||
                cur->IsAsync())
            return false;
        if(cur->dependence_.empty())
            ++front_size;
        if(cur->run_before_.empty())
            ++tail_size;
    }
    return front_size == 1 && tail_size == 1;
}


}
