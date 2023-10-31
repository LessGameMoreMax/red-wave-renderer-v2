#include "TaskGraphNode.h"
namespace sablin{

TaskGraphNode::TaskGraphNode():
        TaskGraphElement(),
        node_type_(TaskGraphNodeType::kBasic){
    element_type_ = TaskGraphElementType::kNode;
    session_ = TaskGraphRandom<>::GenerateSession("node");
}

TaskGraphNode* TaskGraphNode::SetType(const TaskGraphNodeType& type){
    node_type_ = type;
    return this;
}

RStatus TaskGraphNode::Spawn(const GraphTaskGroup& tasks, long ttl){
    ASSERT_NO_STRING(is_init_ == true)
    return thread_pool_->Submit(tasks, ttl);
}
}
