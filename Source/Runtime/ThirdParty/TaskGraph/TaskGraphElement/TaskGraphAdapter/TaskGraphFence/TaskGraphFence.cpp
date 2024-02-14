#include "TaskGraphFence.h"
namespace sablin{

TaskGraphFence::TaskGraphFence():
    TaskGraphAdapter(){
    element_type_ = TaskGraphElementType::kFence;
    session_ = TaskGraphRandom<>::GenerateSession("fence");
}

RStatus TaskGraphFence::CheckSuitable(){
    for(auto* element: fence_elements_){
        ASSERT_NO_STRING(element != nullptr)
        if(!element->IsAsync()) return RStatus("[" + element->name_ + "] Is Not Async Node! Please Set Timeout Value If You Want To Fence It!");
    }
    return RStatus();
}

void TaskGraphFence::DumpElement(std::ostream& oss){
    DumpElementHeader(oss);
    DumpPerfInfo(oss);
    oss << "\", shape=box]\n";
}

RStatus TaskGraphFence::Run(){
    RStatus status;
    for(auto* element: fence_elements_)
        status += element->GetAsyncResult();
    return status;
}

TaskGraphFence* TaskGraphFence::WaitTaskGraphElement(TaskGraphElement* element){
    ASSERT_NO_STRING(is_init_ == false)
    ASSERT_NO_STRING(element != nullptr)
    if(!element->IsAsync()){
        std::cout << "Fence Can Add Async Element Only. Please Set Timeout Value For [" + element->GetName() + "] if You Need." << std::endl;
        ASSERT_NO_STRING(false)
    }
    fence_elements_.insert(element);
    return this;
}

TaskGraphFence* TaskGraphFence::WaitTaskGraphElements(const std::set<TaskGraphElement*>& elements){
    ASSERT_NO_STRING(is_init_ == false)
    for(auto* element: elements)
        WaitTaskGraphElement(element);
    return this;
}

void TaskGraphFence::Clear(){
    ASSERT_NO_STRING(is_init_ == false)
    fence_elements_.clear();
}

}
