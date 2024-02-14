#ifndef TASK_GRAPH_ADAPTER_H
#define TASK_GRAPH_ADAPTER_H
#include "../TaskGraphElement.h"
namespace sablin{

class TaskGraphAdapter: public TaskGraphElement{
    friend class TaskGraphPipeline;
protected:
    explicit TaskGraphAdapter():
        TaskGraphElement(){
        element_type_ = TaskGraphElementType::kAdapter;
    }
};

}
#endif
