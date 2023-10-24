#ifndef TASK_GRAPH_ELEMENT_SORTER_H
#define TASK_GRAPH_ELEMENT_SORTER_H
#include "TaskGraphElement.h"
namespace sablin{

class TaskGraphElementSorter{
public:
    bool operator()(TaskGraphElement* a, TaskGraphElement* b) const{
        return (a->level_ == b->level_)? (a < b): (a->level_ < b->level_);
    }
};

}
#endif
