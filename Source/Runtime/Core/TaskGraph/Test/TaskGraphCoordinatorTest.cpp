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
    TaskGraphElement* e = nullptr;
    TaskGraphElement* f = nullptr;
    TaskGraphElement* g = nullptr;
    TaskGraphElement* h = nullptr;
    TaskGraphCoordinator<-2>* e_coordinator = nullptr;

    GraphTaskThreadPoolConfig config;
    config.secondary_thread_size_ = 4;
    pipeline->SetUniqueThreadPoolConfig(config);

    pipeline->RegisterTaskGraphElement<MyNode1>(&a, {}, "nodeA");
    pipeline->RegisterTaskGraphElement<MyNode1>(&b, {}, "nodeB");
    pipeline->RegisterTaskGraphElement<MyNode1>(&c, {}, "nodeC");
    pipeline->RegisterTaskGraphElement<MyNode1>(&d, {}, "nodeD");

    pipeline->RegisterTaskGraphElement<TaskGraphCoordinator<-2>>(&e_coordinator, {a, b, c, d},
            "coordinatorE");
    pipeline->RegisterTaskGraphElement<MyNode1>(&f, {e_coordinator}, "nodeF");
    pipeline->RegisterTaskGraphElement<MyNode2>(&g, {f}, "nodeG");
    pipeline->RegisterTaskGraphElement<MyNode2>(&h, {f}, "nodeH");


    RStatus status = pipeline->Process();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
