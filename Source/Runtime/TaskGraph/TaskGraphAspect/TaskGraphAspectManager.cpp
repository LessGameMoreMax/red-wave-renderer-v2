#include "TaskGraphAspectManager.h"
namespace sablin{

TaskGraphAspectManager::TaskGraphAspectManager():
    TaskGraphAspectBase(), TaskGraphManagerBase<TaskGraphAspect>(){}

TaskGraphAspectManager::~TaskGraphAspectManager(){
    Clear();
}

RStatus TaskGraphAspectManager::Clear(){
    for(TaskGraphAspect* aspect: aspect_arr_)
        if(aspect != nullptr) delete aspect;
    aspect_arr_.clear();
    return RStatus();
}

RStatus TaskGraphAspectManager::PopLast(){
    if(GetSize() == 0)
        return RStatus("No Aspect To Pop!", STRING_CODE_LOCATION);
    TaskGraphAspect* last = aspect_arr_.back();
    if(last != nullptr) delete last;
    aspect_arr_.pop_back();
    return RStatus();
}

size_t TaskGraphAspectManager::GetSize() const{
    return aspect_arr_.size();
}

RStatus TaskGraphAspectManager::Add(TaskGraphAspect* aspect){
    ASSERT_NO_STRING(aspect != nullptr)
    aspect_arr_.emplace_back(aspect);
    return RStatus();
}

TaskGraphAspectManager* TaskGraphAspectManager::SetName(const std::string& name){
    for(TaskGraphAspect* aspect: aspect_arr_){
        ASSERT_NO_STRING(aspect != nullptr)
        aspect->SetName(name);
    }
    return this;
}

RStatus TaskGraphAspectManager::Reflect(const TaskGraphAspectType& type, const RStatus& cur_status){
    RStatus status;
    for(TaskGraphAspect* aspect: aspect_arr_){
        switch(type){
            case TaskGraphAspectType::kBeginRun: status = aspect->BeginRun(); break;
            case TaskGraphAspectType::kFinishRun: aspect->FinishRun(cur_status); break;
            case TaskGraphAspectType::kBeginInit: status = aspect->BeginInit(); break;
            case TaskGraphAspectType::kFinishInit: aspect->FinishInit(cur_status); break;
            case TaskGraphAspectType::kBeginDestroy: status = aspect->BeginDestroy(); break;
            case TaskGraphAspectType::kFinishDestroy: aspect->FinishDestroy(cur_status); break;
            case TaskGraphAspectType::kEnterCrashed: aspect->EnterCrashed(); break;
            default: status = RStatus("Unknown Aspect Type!", STRING_CODE_LOCATION);
        }
        ASSERT_NO_STRING(status.IsOk())
    }
    return status;
}
}
