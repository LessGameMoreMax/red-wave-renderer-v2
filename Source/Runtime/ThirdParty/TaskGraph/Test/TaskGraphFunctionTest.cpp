#include <iostream>
#include "../../Core/HAL/MemoryManager.h"
#include "../TaskGraphElement/TaskGraphNode/TaskGraphNode.h"
#include "../TaskGraphPipeline/TaskGraphPipeline.h"
#include "../TaskGraphPipeline/TaskGraphPipelineFactory.h"
#include "../../Core/Utility/ThreadSafeIostream.h"
#include "../TaskGraphElement/TaskGraphGroup/TaskGraphCluster/TaskGraphCluster.h"
#include "../TaskGraphAspect/TaskGraphAspect.h"
#include "../TaskGraphAspect/TaskGraphTemplateAspect.h"
#include "../TaskGraphParam/TaskGraphParam.h"
#include "../TaskGraphElement/TaskGraphAdapter/TaskGraphFunction/TaskGraphFunction.h"

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

class MyWriteParamNode: public TaskGraphNode{
public:
    RStatus Init() override{
        RStatus status;
        status = CreateParam<MyParam>("param1");
        return status;
    }

    RStatus Run() override{
        auto* my_param = GetParamWithNoEmpty<MyParam>("param1");
        int val = 0;
        int cnt = 0;
        {
            std::shared_lock<std::shared_mutex>(my_param->param_shared_lock_);
            val = ++my_param->i_value;
            cnt = ++my_param->i_count;
        }
        ThreadSafePrintf("[%s], iValue value is: [%d], iCount value is [%d] ...", GetName().c_str(),
                val, cnt);
        return RStatus();
    }
};

int main(){
    MemoryManagerRAII memory_manager_raii;
    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    TaskGraphElement* a = nullptr;
    TaskGraphElement* b = nullptr;
    TaskGraphFunction* c_function = nullptr;
    TaskGraphFunction* d_function = nullptr;
    pipeline->RegisterTaskGraphElement<MyNode1>(&a, {}, "nodeA", 1);
    pipeline->RegisterTaskGraphElement<MyWriteParamNode>(&b, {a}, "nodeB", 1);
    pipeline->RegisterTaskGraphElement<TaskGraphFunction>(&c_function, {b}, "functionC", 1);
    pipeline->RegisterTaskGraphElement<TaskGraphFunction>(&d_function, {c_function}, "functionD", 3);

    int num = 10;
    const std::string& info = "Hello TaskGraph!";
    c_function->SetFunction(TaskGraphFunctionType::kRun, [num, info] {
            ThreadSafePrintf("input num i = [%d], info = [%s]", num, info.c_str());
            return RStatus();
    });

    d_function->SetFunction(TaskGraphFunctionType::kInit, [d_function] {
            ThreadSafePrintf("[%s] do init function ...", d_function->GetName().c_str());
            return RStatus();
    })->SetFunction(TaskGraphFunctionType::kRun, [d_function, num] {
        auto param = d_function->GetParamWithNoEmpty<MyParam>("param1");
        param->i_count += num;
        ThreadSafePrintf("[%s] do run function, i_count = [%d], iValue = [%d] ...",
            d_function->GetName().c_str(), param->i_count, ++param->i_value);
        return RStatus();
    });
    RStatus status = pipeline->Process();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
