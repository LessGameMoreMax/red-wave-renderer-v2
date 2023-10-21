#include "TaskGraphEventManager.h"
namespace sablin{

TaskGraphEventManager::TaskGraphEventManager():
    TaskGraphEventBase(), TaskGraphManagerBase<TaskGraphEvent>(){}

TaskGraphEventManager::~TaskGraphEventManager(){
    Clear();
}

RStatus TaskGraphEventManager::Setup(){
    RStatus status;
    for(auto& iter: event_map_){
        status += (iter.second)->Setup();
    }
    return status;
}

RStatus TaskGraphEventManager::Exit(){
    RStatus status;
    for(auto& iter: event_map_){
        status += (iter.second)->Exit();
    }
    return status;
}

RStatus TaskGraphEventManager::Clear(){
    for(auto& iter: event_map_){
        delete iter.second;
    }
    event_map_.clear();
    return RStatus();
}

TaskGraphEventBase* TaskGraphEventManager::SetThreadPool(GraphTaskThreadPool* thread_pool){
    ASSERT_NO_STRING(thread_pool != nullptr)
    for(auto& iter: event_map_){
        ASSERT_NO_STRING(iter.second != nullptr)
        iter.second->SetThreadPool(thread_pool);
    }
    return this;
}

RStatus TaskGraphEventManager::Trigger(const std::string& key, TaskGraphEventType event_type){
    auto result = event_map_.find(key);
    if(result == event_map_.end()){
        return RStatus("Event Key [" + key + "] No Find!");
    }
    TaskGraphEvent* event = result->second;
    ASSERT_NO_STRING(event != nullptr)
    return event->Process(event_type);
}


}
