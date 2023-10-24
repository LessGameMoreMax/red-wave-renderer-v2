#ifndef TASK_GRAPH_GROUP_H
#define TASK_GRAPH_GROUP_H
#include "../TaskGraphElement.h"
#include <vector>
namespace sablin{

class TaskGraphGroup: public TaskGraphElement{
    friend class TaskGraphStaticEngine;
    friend class TaskGraphCluster;
private:
    std::vector<TaskGraphElement*> group_elements_arr_;
private:
    explicit TaskGraphGroup();
    virtual RStatus Init() override;
    virtual RStatus Destroy() override;
    void DumpGroupLabelBegin(std::ostream& oss);
    void DumpGroupLabelEnd(std::ostream& oss);
protected:
    virtual RStatus AddElement(TaskGraphElement* element) = 0;
    virtual bool IsSerializable() override;
    bool IsRegistered();
};

}
#endif
