#include <iostream>
#include "../../Core/HAL/MemoryManager.h"
#include "../TaskGraphElement/TaskGraphNode/TaskGraphNode.h"
#include "../TaskGraphPipeline/TaskGraphPipeline.h"
#include "../TaskGraphPipeline/TaskGraphPipelineFactory.h"
#include "../../Core/Utility/ThreadSafeIostream.h"
#include "../TaskGraphElement/TaskGraphGroup/TaskGraphCluster/TaskGraphCluster.h"
#include "../TaskGraphAspect/TaskGraphAspect.h"
#include "../TaskGraphAspect/TaskGraphTemplateAspect.h"

using namespace sablin;
using namespace std;

class MyNode1: public TaskGraphNode{
public:
    RStatus Run() override{
        ThreadSafePrintf("[%s], enter MyNode1 run function. Sleep for 1 second ...\n", this->GetName().c_str());
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return RStatus();
    }
};

class MyNode2: public TaskGraphNode{
public:
    RStatus Init() override{
        ThreadSafePrintf("[INIT] [%s], enter MyNode2 init function.\n", this->GetName().c_str());
        return RStatus();
    }

    RStatus Run() override{
        ThreadSafePrintf("[%s], enter MyNode2 run function. Sleep for 2 second ...\n", this->GetName().c_str());
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return RStatus();
    }

    RStatus Destroy() override{
        ThreadSafePrintf("[DESTROY] [%s], enter MyNode2 destroy function.\n", this->GetName().c_str());
        return RStatus();
    }
};

class MyTimerAspect: public TaskGraphAspect{
public:
    RStatus BeginRun() override{
        start_ts_ = std::chrono::steady_clock::now();
        return RStatus();
    }

    void FinishRun(const RStatus& cur_status) override{
        std::chrono::duration<double, std::milli> span = std::chrono::steady_clock::now() - start_ts_;
        ThreadSafePrintf("---> [MyTimerAspect] [%s] time cost is : [%0.2lf] ms", GetName().c_str(), span.count());
    }
private:
    std::chrono::steady_clock::time_point start_ts_;
};

class MyTraceAspect : public TaskGraphAspect{
public:
    RStatus BeginInit() override {
        ThreadSafePrintf("----> [MyTraceAspect] [%s] init begin ...", GetName().c_str());
        return RStatus();
    }

    void FinishInit(const RStatus& curStatus) override {
        ThreadSafePrintf("----> [MyTraceAspect] [%s] init finished, error code is [%d] ...", this->GetName().c_str(), curStatus.GetErrorCode());
    }

    RStatus BeginRun() override {
        ThreadSafePrintf("----> [MyTraceAspect] [%s] run begin ...", GetName().c_str());
        return RStatus();
    }

    void FinishRun(const RStatus& curStatus) override {
        if (!curStatus.IsOk()) {
            ThreadSafePrintf("----> [MyTraceAspect] [%s] run finished, status is ok ...", GetName().c_str());
        } else {
            ThreadSafePrintf("----> [MyTraceAspect] [%s] run finished, error code is [%d] ...", GetName().c_str(), curStatus.GetErrorCode());
        }
    }

    RStatus BeginDestroy() override {
        ThreadSafePrintf("----> [MyTraceAspect] [%s] destroy begin ...", GetName().c_str());
        return RStatus();
    }

    void FinishDestroy(const RStatus& curStatus) override {
        ThreadSafePrintf("----> [MyTraceAspect] [%s] destroy finished, error code is [%d] ...", GetName().c_str(), curStatus.GetErrorCode());
    }
};

template<class... Args>
class MyTemplateAspect: public TaskGraphTemplateAspect<Args...>{
public:
    explicit MyTemplateAspect(int age, double score){
        age_ = age;
        score_ = score;
    }

    RStatus BeginInit() override{
        ThreadSafePrintf("-----> [MyTemplateAspect] [%s] input age is [%d], score is [%lf]",
                this->GetName().c_str(), age_, score_);
        return RStatus();
    }
private:
    int age_;
    double score_;
};

int main(){
    MemoryManagerRAII memory_manager_raii;
    RStatus status;
    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    TaskGraphElement* a = nullptr;
    TaskGraphElement* b_cluster = nullptr;
    TaskGraphElement* c = nullptr;

    b_cluster = pipeline->CreateTaskGraphGroup<TaskGraphCluster>({
        pipeline->CreateTaskGraphNode<MyNode1>(TaskGraphNodeInfo("nodeB1", 1)),
        pipeline->CreateTaskGraphNode<MyNode1>(TaskGraphNodeInfo("nodeB2", 2)),
    });

    pipeline->RegisterTaskGraphElement<MyNode1>(&a, {}, "nodeA", 1);
    pipeline->RegisterTaskGraphElement<TaskGraphCluster>(&b_cluster, {a}, "clusterB", 1);
    pipeline->RegisterTaskGraphElement<MyNode1>(&c, {b_cluster}, "nodeC", 1);

    a->AddTaskGraphAspect<MyTraceAspect>();
    a->AddTaskGraphAspect<MyTemplateAspect<int, double>>(20, 7.0);
    b_cluster->AddTaskGraphAspect<MyTimerAspect>();
    pipeline->AddTaskGraphAspect<MyTraceAspect>({b_cluster, c});
    pipeline->Process();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
