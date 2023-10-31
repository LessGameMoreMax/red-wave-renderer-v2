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

struct MyParam: public TaskGraphParam{
    void Reset(const RStatus& cur_status) override{
        i_value = 0;
    }
    int i_value = 0;
    int i_count = 0;
};

class MyHoldNode: public TaskGraphNode{
public:
    RStatus Init() override{
        return CreateParam<MyParam>("hold-param");
    };

    RStatus Run() override{
        auto param = GetParam<MyParam>("hold-param");
        ASSERT_NO_STRING(param != nullptr)
        param->i_value++;
        ThreadSafePrintf("iCount Value is [%d]", param->i_value);
        return RStatus();
    }

    bool IsHold() override{
        auto param = GetParam<MyParam>("hold-param");
        if(param == nullptr) return false;
        ThreadSafePrintf("enter hold path, iValue = [%d]", param->i_value);
        return param->i_value < 5;
    }
};

int main(){
    MemoryManagerRAII memory_manager_raii;
    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    TaskGraphElement* hold_node = nullptr;
    TaskGraphElement* node1 = nullptr;
    pipeline->RegisterTaskGraphElement<MyHoldNode>(&hold_node, {}, "myHold");
    pipeline->RegisterTaskGraphElement<MyNode1>(&node1, {hold_node}, "node1");

    RStatus status = pipeline->Process(3);
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
