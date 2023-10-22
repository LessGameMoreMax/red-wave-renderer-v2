#ifndef TASK_GRAPH_ELEMENT_RELATION_H
#define TASK_GRAPH_ELEMENT_RELATION_H
#include <set>

namespace sablin{
class TaskGraphElement;

struct TaskGraphElementRelation{
    std::set<TaskGraphElement*> predecessors_;
    std::set<TaskGraphElement*> successors_;
    TaskGraphElement* belong_ = nullptr;
};

}
#endif
