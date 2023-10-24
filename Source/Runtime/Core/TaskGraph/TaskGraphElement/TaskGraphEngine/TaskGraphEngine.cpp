#include "TaskGraphEngine.h"
namespace sablin{

TaskGraphEngine::TaskGraphEngine():
    thread_pool_(nullptr),
    total_element_size_(0),
    schedule_strategy_(kDefaultGraphTaskStrategy){}

int32_t TaskGraphEngine::CalcIndex(TaskGraphElement* element){
    auto binding_index = element->GetBindingIndex();
    return binding_index == kTaskGraphDefaultBindingIndex? schedule_strategy_: binding_index;
}
}
