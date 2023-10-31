#ifndef TASK_GRAPH_NODE_INFO_H
#define TASK_GRAPH_NODE_INFO_H
#include <string>
#include "../TaskGraphElement.h"
namespace sablin{

struct TaskGraphNodeInfo{
    std::string name_;
    size_t loop_{kTaskGraphDefaultLoopTimes};
    std::set<TaskGraphElement*> dependence_;

    explicit TaskGraphNodeInfo(const std::string& name = STRING_NULL,
                size_t loop = kTaskGraphDefaultLoopTimes){
        name_ = name;
        loop_ = loop;
    }

    explicit TaskGraphNodeInfo(const std::set<TaskGraphElement*>& dependence 
            = std::initializer_list<TaskGraphElement*>(), const std::string& name = STRING_NULL,
            size_t loop = kTaskGraphDefaultLoopTimes){
        name_ = name;
        loop_ = loop;
        dependence_ = dependence;
    }
};

}
#endif
