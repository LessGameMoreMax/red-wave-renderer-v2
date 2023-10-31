#include "TaskGraphDaemonManager.h"
namespace sablin{

TaskGraphDaemonManager::TaskGraphDaemonManager():
    TaskGraphDaemonBase(), TaskGraphManagerBase<TaskGraphDaemon>(){}

TaskGraphDaemonManager::~TaskGraphDaemonManager(){
    Clear();
}

RStatus TaskGraphDaemonManager::Setup(){
    RStatus status;
    for(TaskGraphDaemon* daemon: daemons_){
        ASSERT_NO_STRING(daemon != nullptr)
        status += daemon->Setup();
    }
    return status;
}

RStatus TaskGraphDaemonManager::Exit(){
    RStatus status;
    for(TaskGraphDaemon* daemon: daemons_){
        ASSERT_NO_STRING(daemon != nullptr)
        status += daemon->Exit();
    }
    return status;
}

RStatus TaskGraphDaemonManager::Clear(){
    for(TaskGraphDaemon* daemon: daemons_)
        if(daemon != nullptr) delete daemon;
    daemons_.clear();
    return RStatus();
}

RStatus TaskGraphDaemonManager::Add(TaskGraphDaemon* daemon){
    ASSERT_NO_STRING(daemon != nullptr)
    daemons_.insert(daemon);
    return RStatus();
}

RStatus TaskGraphDaemonManager::Remove(TaskGraphDaemon* daemon){
    ASSERT_NO_STRING(daemon != nullptr)
    daemons_.erase(daemon);
    delete daemon;
    return RStatus();
}

size_t TaskGraphDaemonManager::GetSize() const{
    return daemons_.size();
}

TaskGraphDaemonManager* TaskGraphDaemonManager::SetInterval(long interval){
    if(interval == 0) return this;
    for(TaskGraphDaemon* daemon: daemons_){
        ASSERT_NO_STRING(daemon != nullptr)
        daemon->SetInterval(interval);
    }
    return this;
}


}
