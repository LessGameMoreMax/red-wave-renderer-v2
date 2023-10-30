#include <iostream>
#include "../../HAL/MemoryManager.h"
#include "../TaskGraphElement/TaskGraphNode/TaskGraphNode.h"
#include "../TaskGraphPipeline/TaskGraphPipeline.h"
#include "../TaskGraphPipeline/TaskGraphPipelineFactory.h"
#include "../../Utility/ThreadSafeIostream.h"
#include "../TaskGraphElement/TaskGraphGroup/TaskGraphCluster/TaskGraphCluster.h"

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
    RStatus status;
    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    TaskGraphElement* a = nullptr;
    TaskGraphElement* b_cluster = nullptr;
    TaskGraphElement* c = nullptr;
    TaskGraphElement* d = nullptr;

    b_cluster = pipeline->CreateTaskGraphGroup<TaskGraphCluster>({
        pipeline->CreateTaskGraphNode<MyNode1>(TaskGraphNodeInfo("nodeB1", 1)),
        pipeline->CreateTaskGraphNode<MyNode1>(TaskGraphNodeInfo("nodeB2", 3)),
        pipeline->CreateTaskGraphNode<MyNode2>(TaskGraphNodeInfo("nodeB3", 1))
    });
    status = pipeline->RegisterTaskGraphElement<MyNode1>(&a, {}, "nodeA", 1);
    status += pipeline->RegisterTaskGraphElement<TaskGraphCluster>(&b_cluster, {a}, "clusterB", 2);
    status += pipeline->RegisterTaskGraphElement<MyNode1>(&c, {a}, "nodeC", 1);
    status += pipeline->RegisterTaskGraphElement<MyNode2>(&d, {b_cluster, c}, "nodeD", 2);
    status += pipeline->Process();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
