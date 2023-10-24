#ifndef TASK_GRAPH_CLUSTER_H
#define TASK_GRAPH_CLUSTER_H
#include "../TaskGraphGroup.h"
namespace sablin{

class TaskGraphCluster: public TaskGraphGroup{
    friend class TaskGraphStaticEngine;
private:
    RStatus Process(bool is_mock);
    virtual RStatus Run() final;
    RStatus AddElement(TaskGraphElement* element) final;
    virtual RStatus BeforeRun() final;
    virtual RStatus AfterRun() final;
    virtual void Dump(std::ostream& oss) final;
    bool IsDone();
protected:
    explicit TaskGraphCluster();
    size_t GetElementNum();
    virtual int32_t GetBindingIndex() override;
public:
    TaskGraphCluster(const TaskGraphCluster& cluster);
    TaskGraphCluster& operator=(const TaskGraphCluster& cluster);
};

}
#endif
