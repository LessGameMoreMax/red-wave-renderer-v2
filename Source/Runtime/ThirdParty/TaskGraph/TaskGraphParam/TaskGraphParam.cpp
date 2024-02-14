#include "TaskGraphParam.h"
namespace sablin{

TaskGraphParam::TaskGraphParam(): backtrace_enable_(false) {}

RStatus TaskGraphParam::GetBackTrace(std::vector<std::string>& traces){
    if(!backtrace_enable_)[[likely]] return RStatus("BackTrace No Enable!", STRING_CODE_LOCATION);
    backtrace_lock_.Lock();
    traces.clear();
    backtrace_.GetUniqueArray(traces);
    backtrace_lock_.UnLock();
    return RStatus();
}

void TaskGraphParam::AddBackTrace(const std::string& trace){
    if(!backtrace_enable_)[[likely]] return;
    backtrace_lock_.Lock();
    backtrace_.UniqueAdd(trace);
    backtrace_lock_.UnLock();
}

void TaskGraphParam::CleanBackTrace(){
    if(!backtrace_enable_)[[likely]] return;
    backtrace_lock_.Lock();
    backtrace_.Clear();
    backtrace_lock_.UnLock();
}

const std::string& TaskGraphParam::GetKey() const{
    return key_;
}

}
