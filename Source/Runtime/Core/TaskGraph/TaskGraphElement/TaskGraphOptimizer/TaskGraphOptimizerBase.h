#ifndef TASK_GRAPH_OPTIMIZER_BASE_H
#define TASK_GRAPH_OPTIMIZER_BASE_H
#include <set>
#include "../TaskGraphElement.h"
#include "../TaskGraphElementSorter.h"
namespace sablin{

class TaskGraphOptimizerBase{
protected:
    virtual bool Match(const std::set<TaskGraphElement*, TaskGraphElementSorter>& elements) = 0;
};
    
}
#endif
