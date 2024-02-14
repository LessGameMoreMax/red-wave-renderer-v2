#include "TaskGraphAspect.h"
namespace sablin{

RStatus TaskGraphAspect::BeginInit(){
    return RStatus();
}

void TaskGraphAspect::FinishInit(const RStatus& cur_status){}

RStatus TaskGraphAspect::BeginRun(){
    return RStatus();
}

void TaskGraphAspect::FinishRun(const RStatus& cur_status){}

RStatus TaskGraphAspect::BeginDestroy(){
    return RStatus();
}

void TaskGraphAspect::FinishDestroy(const RStatus& cur_status){}

void TaskGraphAspect::EnterCrashed(){}

}
