#ifndef TASK_GRAPH_ELEMENT_MANAGER_H
#define TASK_GRAPH_ELEMENT_MANAGER_H
#include "../../HAL/Runnable.h"
#include "../TaskGraphManagerBase.h"
#include "TaskGraphElement.h"
#include "TaskGraphElementSorter.h"
#include "TaskGraphEngine/TaskGraphEngine.h"
#include "TaskGraphEngine/TaskGraphStaticEngine/TaskGraphStaticEngine.h"
#include "TaskGraphEngine/TaskGraphDynamicEngine/TaskGraphDynamicEngine.h"
namespace sablin{

class TaskGraphElementManager: public Runnable,
                               public TaskGraphManagerBase<TaskGraphElement>{
private:
    std::set<TaskGraphElement*, TaskGraphElementSorter> manager_elements_;
    TaskGraphEngine* engine_;
    TaskGraphEngineType engine_type_;
    GraphTaskThreadPool* thread_pool_;
    bool auto_check_enable_;
protected:
    explicit TaskGraphElementManager();
    CLASS_NO_ALLOWED_COPY(TaskGraphElementManager)
    virtual ~TaskGraphElementManager();

    virtual RStatus Init() override;
    virtual RStatus Run() override;
    virtual RStatus Destroy() override;

    virtual RStatus Add(TaskGraphElement* element) override;
    virtual RStatus Remove(TaskGraphElement* element) override;
    virtual bool Find(TaskGraphElement* element) const override;
    virtual RStatus Clear() final;

    TaskGraphElementManager* SetScheduleStrategy(int32_t strategy);
    TaskGraphElementManager* SetEngineType(TaskGraphEngineType engine_type);
    RStatus InitEngine();
    TaskGraphElementManager* SetThreadPool(GraphTaskThreadPool* ptr);
    RStatus CalcMaxParaSize(size_t& size);
    bool CheckSerializable();

};

}
#endif
