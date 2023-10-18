#ifndef GRAPH_TASK_H
#define GRAPH_TASK_H
#include "../../HAL/Runnable.h"
#include "../../Misc/MacroTools.h"
#include "../../Debug/Assertion.h"
namespace sablin{

class GraphTask: public Runnable{
private:
    struct GraphTaskBased{
        explicit GraphTaskBased() = default;
        virtual ~GraphTaskBased() = default;
        virtual void Call() = 0;

    };

    template <typename F, typename T = typename std::decay<F>::type>
    struct GraphTaskDerided: public GraphTaskBased{
        T func_;
        explicit GraphTaskDerided(F&& func): func_(std::forward<F>(func)){}
        virtual void Call() override { func_();}
    };
private:
    GraphTaskBased* impl_;
    int32_t priority_;
public:
    GraphTask(): impl_(nullptr), priority_(0){}

    template <typename F>
    GraphTask(F&& f, int32_t priority = 0): 
        impl_(new GraphTaskDerided<F>(std::forward<F>(f))),
        priority_(priority){}

    virtual ~GraphTask(){
        if(impl_ != nullptr) {delete impl_;}
        impl_ = nullptr;
        priority_ = 0;
    }

    CLASS_NO_ALLOWED_COPY(GraphTask)

    GraphTask(GraphTask&& task) noexcept: priority_(task.priority_){
        impl_ = task.impl_;
        task.impl_ = nullptr;
        task.priority_ = 0;
    }

    GraphTask& operator=(GraphTask&& task) noexcept{
        impl_ = task.impl_;
        priority_ = task.priority_;
        task.impl_ = nullptr;
        task.priority_ = 0;
        return *this;
    }

    bool operator>(const GraphTask& task) const{
        return priority_ < task.priority_;
    }

    bool operator<(const GraphTask& task) const{
        return priority_ >= task.priority_;
    }

    void operator()(){
        ASSERT_NO_STRING(impl_ != nullptr)
        impl_->Call();
    }
};

}
#endif
