#include <iostream>
#include "../../HAL/MemoryManager.h"
#include "../TaskGraphElement/TaskGraphNode/TaskGraphNode.h"
#include "../TaskGraphPipeline/TaskGraphPipeline.h"
#include "../TaskGraphPipeline/TaskGraphPipelineFactory.h"
#include "../../Utility/ThreadSafeIostream.h"
#include "../TaskGraphElement/TaskGraphGroup/TaskGraphCluster/TaskGraphCluster.h"
#include "../TaskGraphAspect/TaskGraphAspect.h"
#include "../TaskGraphAspect/TaskGraphTemplateAspect.h"
#include "../TaskGraphParam/TaskGraphParam.h"

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


int main(){
    MemoryManagerRAII memory_manager_raii;
    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    TaskGraphFunction* a_function = nullptr;
    TaskGraphElement* b = nullptr;
    TaskGraphElement* c = nullptr;
    TaskGraphFence* d_fence = nullptr;
    TaskGraphElement* e = nullptr;

    pipeline->RegisterTaskGraphElement<TaskGraphFunction>(&a_function, {}, "functionA");
    pipeline->RegisterTaskGraphElement<MyNode2>(&b, {a_function}, "nodeB");
    pipeline->RegisterTaskGraphElement<MyNode1>(&c, {a_function}, "nodeC");
    pipeline->RegisterTaskGraphElement<TaskGraphFence>(&d_fence, {b, c}, "fenceD");
    pipeline->RegisterTaskGraphElement<MyNode1>(&e, {d_fence}, "nodeE");

    long sec = 5;
    a_function->SetFunction(TaskGraphFunctionType::kRun, [a_function, sec]{
            ThreadSafePrintf("[%s] begine sleep for [%ld]s", a_function->GetName().c_str(), sec);
            this_thread::sleep_for(chrono::seconds(sec));
            ThreadSafePrintf("[%s] run finished.", a_function->GetName().c_str());
            return RStatus();
    });

    a_function->SetTimeout(200, TaskGraphElementTimeoutStrategy::kHoldByPipeline);
    b->SetTimeout(200, TaskGraphElementTimeoutStrategy::kHoldByPipeline);

    d_fence->WaitTaskGraphElements({a_function, b});

    RStatus status = pipeline->Process();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
