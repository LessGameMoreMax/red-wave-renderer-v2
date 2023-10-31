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
#include "../TaskGraphElement/TaskGraphAdapter/TaskGraphCoordinator/TaskGraphCoordinator.h"

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
    TaskGraphElement* a = nullptr;
    TaskGraphElement* b = nullptr;
    TaskGraphElement* c = nullptr;
    TaskGraphElement* d = nullptr;

    pipeline->RegisterTaskGraphElement<MyNode1>(&a, {}, "nodeA");
    pipeline->RegisterTaskGraphElement<MyNode2>(&b, {a}, "nodeB", 3);
    pipeline->RegisterTaskGraphElement<MyNode2>(&c, {a}, "nodeC", 3);
    pipeline->RegisterTaskGraphElement<MyNode2>(&d, {b, c}, "nodeD");

    pipeline->Init();
    ThreadSafePrintf("Pipeline Async Run, Begin!");
    auto result = pipeline->AsyncRun();
    this_thread::sleep_for(chrono::milliseconds(2600));
    ThreadSafePrintf("Pipeline Async Run, Yield!");
    pipeline->Yield();
    this_thread::sleep_for(chrono::milliseconds(7200));
    ThreadSafePrintf("Pipeline Async Run, Resume after 7200ms!");
    pipeline->Resume();
    result.Wait();
    pipeline->Destroy();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
