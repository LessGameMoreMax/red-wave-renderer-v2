#include <iostream>
#include "../../Core/HAL/MemoryManager.h"
#include "../TaskGraphElement/TaskGraphNode/TaskGraphNode.h"
#include "../TaskGraphPipeline/TaskGraphPipeline.h"
#include "../TaskGraphPipeline/TaskGraphPipelineFactory.h"
#include "../../Core/Utility/ThreadSafeIostream.h"

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
    RStatus status = pipeline->RegisterTaskGraphElement<MyNode1>(&a, {}, "nodeA");
    status += pipeline->RegisterTaskGraphElement<MyNode2>(&b, {a}, "nodeB");
    status += pipeline->RegisterTaskGraphElement<MyNode1>(&c, {a}, "nodeC");
    status += pipeline->RegisterTaskGraphElement<MyNode2>(&d, {b, c}, "nodeD");
    status += pipeline->Process();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
