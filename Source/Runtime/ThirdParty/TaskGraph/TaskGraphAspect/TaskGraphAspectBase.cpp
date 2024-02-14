#include "TaskGraphAspectBase.h"
namespace sablin{

TaskGraphAspectBase::TaskGraphAspectBase():
    aspect_param_ptr_(nullptr),
    param_manager_(nullptr),
    event_manager_(nullptr){
    session_ = TaskGraphRandom<>::GenerateSession("aspect");
}

TaskGraphAspectBase::~TaskGraphAspectBase(){
    if(aspect_param_ptr_ != nullptr){
        delete aspect_param_ptr_;
        aspect_param_ptr_ = nullptr;
    }
}

}
