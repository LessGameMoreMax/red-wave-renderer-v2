#include "TaskGraphGroup.h"
namespace sablin{

TaskGraphGroup::TaskGraphGroup(): TaskGraphElement(){
    element_type_ = TaskGraphElementType::kGroup;
}

RStatus TaskGraphGroup::Init(){
    RStatus status;
    for(TaskGraphElement* element: group_elements_arr_){
        ASSERT_NO_STRING(element != nullptr)
        status += element->FatProcessor(TaskGraphFunctionType::kInit);
    }
    ASSERT_NO_STRING(status.IsOk())
    is_init_ = true;
    return status;
}

RStatus TaskGraphGroup::Destroy(){
    RStatus status;
    for(TaskGraphElement* element: group_elements_arr_){
        ASSERT_NO_STRING(element != nullptr)
        status += element->FatProcessor(TaskGraphFunctionType::kDestroy);
    }
    is_init_ = false;
    return status;
}

void TaskGraphGroup::DumpGroupLabelBegin(std::ostream& oss){
    oss << "subgraph ";
    oss << "cluster_p" << this;
    oss << " {\nlabel=\"";
    if(name_.empty()){
        oss << 'p' << this;
    }else{
        oss << name_;
    }
    if(loop_ > 1){
        oss << "(loop=" << loop_ << ")";
    }
    DumpPerfInfo(oss);
    oss << "\";\n";
}

void TaskGraphGroup::DumpGroupLabelEnd(std::ostream& oss){
    oss << "}\n";
}

bool TaskGraphGroup::IsSerializable(){
    return std::all_of(group_elements_arr_.begin(), group_elements_arr_.end(),
            [](TaskGraphElement* element){return element->IsSerializable(); });
}

bool TaskGraphGroup::IsRegistered(){
    return param_manager_ != nullptr && event_manager_ != nullptr;
}

}
