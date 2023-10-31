#ifndef TASK_GRAPH_DYNAMIC_ENGINE_H
#define TASK_GRAPH_DYNAMIC_ENGINE_H
#include "../TaskGraphEngine.h"
namespace sablin{

class TaskGraphDynamicEngine: public TaskGraphEngine{
    friend class TaskGraphElementManager;
private:
    std::vector<TaskGraphElement*> total_element_arr_;
    std::vector<TaskGraphElement*> front_element_arr_;
    size_t total_end_size_;
    size_t finished_end_size_;
    std::atomic<size_t> run_element_size_;
    RStatus cur_status_;
    std::mutex lock_;
    std::condition_variable cv_;
protected:
    explicit TaskGraphDynamicEngine();
    RStatus Setup(const std::set<TaskGraphElement*, TaskGraphElementSorter>& elements) override;
    virtual RStatus Run() override;
    virtual RStatus AfterRunCheck() override;
    void AsyncRun();
    RStatus BeforeRun();
    RStatus Process(TaskGraphElement* element, bool affinity);
    void AfterElementRun(TaskGraphElement* element);
    void Wait();
};

}
#endif
