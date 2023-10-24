#ifndef TASK_GRAPH_STATIC_ENGINE_H
#define TASK_GRAPH_STATIC_ENGINE_H
#include "../TaskGraphEngine.h"
#include "../../TaskGraphGroup/TaskGraphCluster/TaskGraphCluster.h"
namespace sablin{

class TaskGraphStaticEngine: public TaskGraphEngine{
    friend class TaskGraphElementManager;
private:
    std::vector<std::vector<TaskGraphCluster>> para_cluster_arrs_;
    uint32_t run_element_size_;
protected:
    explicit TaskGraphStaticEngine();
    virtual RStatus Setup(const std::set<TaskGraphElement*, 
            TaskGraphElementSorter>& elements) override;
    virtual RStatus Run() override;
    virtual RStatus AfterRunCheck() override;
    RStatus Mark(const std::set<TaskGraphElement*,
            TaskGraphElementSorter>& elements);
    RStatus Analyse(const std::set<TaskGraphElement*,
            TaskGraphElementSorter>& elemtns);
};

}
#endif
