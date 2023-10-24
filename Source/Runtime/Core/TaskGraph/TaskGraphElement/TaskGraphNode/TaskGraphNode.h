#ifndef TASK_GRAPH_NODE_H
#define TASK_GRAPH_NODE_H
#include "../TaskGraphElement.h"
#include "../../TaskGraphDefine.h"
#include "../../GraphTaskThreadPoolDefine.h"
#include "../../GraphTask/GraphTaskGroup.h"
namespace sablin{

class TaskGraphNode: public TaskGraphElement{
private:
    TaskGraphNodeType node_type_;
protected:
    explicit TaskGraphNode();
    TaskGraphNode* SetType(const TaskGraphNodeType& type);
    RStatus Spawn(const GraphTaskGroup& tasks, long ttl = kMaxBlockTTL);
};

}
#endif
