#include "TaskGraphCluster.h"
namespace sablin{

TaskGraphCluster::TaskGraphCluster(): TaskGraphGroup(){
    element_type_ = TaskGraphElementType::kCluster;
    session_ = TaskGraphRandom<>::GenerateSession("cluster");
}

TaskGraphCluster::TaskGraphCluster(const TaskGraphCluster& cluster){
    group_elements_arr_ = cluster.group_elements_arr_;
}

TaskGraphCluster& TaskGraphCluster::operator=(const TaskGraphCluster& cluster){
    if(this == &cluster){
        return *this;
    }
    group_elements_arr_ = cluster.group_elements_arr_;
    return *this;
}

RStatus TaskGraphCluster::Run(){
    RStatus status;
    for(TaskGraphElement* element: group_elements_arr_){
        status = element->FatProcessor(TaskGraphFunctionType::kRun);
        if(!status.IsOk()) return RStatus("TaskGraphCluster::Run", STRING_CODE_LOCATION);
    }
    return status;
}

RStatus TaskGraphCluster::Process(bool is_mock){
    RStatus status = BeforeRun();
    if(!status.IsOk()) return RStatus("TaskGraphCluster::Process, STRING_CODE_LOCATION");
    if(!is_mock)[[likely]]{
        for(TaskGraphElement* element: group_elements_arr_){
            status = element->FatProcessor(TaskGraphFunctionType::kRun);
            if(!status.IsOk()) return RStatus("TaskGraphCluster::Process", STRING_CODE_LOCATION);
        }
    }
    status = AfterRun();
    return status;
}

RStatus TaskGraphCluster::BeforeRun(){
    RStatus status;
    done_ = false;
    left_depend_ = dependence_.size();
    for(TaskGraphElement* element: group_elements_arr_){
        status = element->BeforeRun();
        if(!status.IsOk()) return RStatus("TaskGraphCluster::BeforeRun", STRING_CODE_LOCATION);
    }
    return status;
}

RStatus TaskGraphCluster::AfterRun(){
    RStatus status;
    for(TaskGraphElement* element: group_elements_arr_){
        status = element->AfterRun();
        if(!status.IsOk()) return RStatus("TaskGraphCluster::AfterRun", STRING_CODE_LOCATION);
    }
    for(auto& element: run_before_){
        --element->left_depend_;
    }
    done_ = true;
    return status;
}

RStatus TaskGraphCluster::AddElement(TaskGraphElement* element){
    ASSERT_NO_STRING(element != nullptr)
    group_elements_arr_.emplace_back(element);
    return RStatus();
}

size_t TaskGraphCluster::GetElementNum(){
    return group_elements_arr_.size();
}

bool TaskGraphCluster::IsDone(){
    return std::all_of(group_elements_arr_.begin(), group_elements_arr_.end(),
            [](TaskGraphElement* element){return element->done_; });
}

int32_t TaskGraphCluster::GetBindingIndex(){
    if(group_elements_arr_.empty()) return kTaskGraphDefaultBindingIndex;
    return group_elements_arr_[0]->GetBindingIndex();
}

void TaskGraphCluster::Dump(std::ostream& oss){
    DumpElement(oss);
    DumpGroupLabelBegin(oss);
    oss << 'p' << this << "[shape=point height=0];\n";
    oss << "color=blue;\n";

    TaskGraphElement* pre = nullptr;
    for(size_t i = 0;i < group_elements_arr_.size(); ++i){
        TaskGraphElement* element = group_elements_arr_[i];
        element->Dump(oss);
        if(i != 0){
            DumpEdge(oss, pre, element);
        }
        pre = element;
    }
    DumpGroupLabelEnd(oss);
    for(const auto& node: run_before_){
        DumpEdge(oss, this, node);
    }
}

}
