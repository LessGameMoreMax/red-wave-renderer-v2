#ifndef TASK_GRAPH_TEMPLATE_NODE_H
#define TASK_GRAPH_TEMPLATE_NODE_H
#include "TaskGraphNode.h"
namespace sablin{

template<typename... Args>
class TaskGraphTemplateNode: public TaskGraphNode{};

}
#endif
