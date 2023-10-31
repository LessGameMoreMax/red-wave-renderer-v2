#include <iostream>
#include "../../Core/HAL/MemoryManager.h"
#include "../TaskGraphElement/TaskGraphNode/TaskGraphNode.h"
#include "../TaskGraphPipeline/TaskGraphPipeline.h"
#include "../TaskGraphPipeline/TaskGraphPipelineFactory.h"
#include "../../Core/Utility/ThreadSafeIostream.h"
#include "../TaskGraphParam/TaskGraphParam.h"

using namespace sablin;
using namespace std;

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

class MyReadParamNode: public TaskGraphNode{
public:
    RStatus Run() override{
        auto my_param = GetParamWithNoEmpty<MyParam>("param1");
        int val = 0;
        {
            std::shared_lock<std::shared_mutex>(my_param->param_shared_lock_);
            val = my_param->i_value;
        }
        ThreadSafePrintf("[%s], iValue is : [%d] ...", GetName().c_str(), val);
        return RStatus();
    }
};

int main(){
    MemoryManagerRAII memory_manager_raii;
    RStatus status;
    TaskGraphElement* a = nullptr;
    TaskGraphElement* b = nullptr;
    TaskGraphElement* c = nullptr;
    TaskGraphElement* d = nullptr;
    TaskGraphElement* e = nullptr;
    TaskGraphElement* f = nullptr;

    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    status += pipeline->RegisterTaskGraphElement<MyReadParamNode>(&a, {}, "readNodeA");
    status += pipeline->RegisterTaskGraphElement<MyReadParamNode>(&b, {a}, "readNodeB");
    status += pipeline->RegisterTaskGraphElement<MyWriteParamNode>(&c, {a}, "writeNodeC");
    status += pipeline->RegisterTaskGraphElement<MyWriteParamNode>(&d, {a}, "writeNodeD", 2);
    status += pipeline->RegisterTaskGraphElement<MyReadParamNode>(&e, {a}, "readNodeE");
    status += pipeline->RegisterTaskGraphElement<MyWriteParamNode>(&f, {b, c, d, e}, "writeNodeF");
    pipeline->Process();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
