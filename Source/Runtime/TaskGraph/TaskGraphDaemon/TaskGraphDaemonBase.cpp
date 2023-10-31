#include "TaskGraphDaemonBase.h"
namespace sablin{

TaskGraphDaemonBase::TaskGraphDaemonBase():
    param_manager_(nullptr), event_manager_(nullptr),
    daemon_param_ptr_(nullptr), interval_(0){
    session_ = TaskGraphRandom<>::GenerateSession("daemon");
}

TaskGraphDaemonBase::~TaskGraphDaemonBase(){
    if(daemon_param_ptr_ != nullptr)
        delete daemon_param_ptr_;
}

TaskGraphDaemonBase* TaskGraphDaemonBase::SetInterval(long interval){
    if(interval == 0) return this;
    interval_ = interval;
    return this;
}
}
