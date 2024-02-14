#include "TaskGraphDaemon.h"
namespace sablin{

RStatus TaskGraphDaemon::Setup(){
    timer_.Start(interval_, [this]{this->DaemonTask(daemon_param_ptr_); });
    return RStatus();
}

RStatus TaskGraphDaemon::Exit(){
    timer_.Stop();
    return RStatus();
}

long TaskGraphDaemon::GetInterval() const{
    return interval_;
}

}
