#include "TaskGraphFunction.h"
namespace sablin{
    
TaskGraphFunction::TaskGraphFunction():
    TaskGraphAdapter(),
    init_function_(nullptr),
    run_function_(nullptr),
    destroy_function_(nullptr){
    element_type_ = TaskGraphElementType::kFunction;
    session_ = TaskGraphRandom<>::GenerateSession("function");
}

RStatus TaskGraphFunction::Init(){
    return init_function_ != nullptr? init_function_(): RStatus();
}

RStatus TaskGraphFunction::Run(){
    return run_function_ != nullptr? run_function_(): RStatus();
}

RStatus TaskGraphFunction::Destroy(){
    return destroy_function_ != nullptr? destroy_function_(): RStatus();
}

TaskGraphFunction* TaskGraphFunction::SetFunction(const TaskGraphFunctionType& type,
        const std::function<RStatus()>& func){
    ASSERT_NO_STRING(is_init_ == false)
    ASSERT_NO_STRING(func != nullptr)

    switch(type){
        case TaskGraphFunctionType::kInit: init_function_ = func; break;
        case TaskGraphFunctionType::kRun: run_function_ = func; break;
        case TaskGraphFunctionType::kDestroy: destroy_function_ = func; break;
        default: return nullptr;
    }
    return this;
}
}
