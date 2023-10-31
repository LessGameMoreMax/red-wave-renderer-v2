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
#include "../TaskGraphEvent/TaskGraphEvent.h"

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

class MyEventNode: public TaskGraphNode{
public:
    RStatus Run() override{
        ThreadSafePrintf("[%s], before event notify", GetName().c_str());
        Notify("my-print-event", TaskGraphEventType::kTaskGraphEventTypeSync);
        ThreadSafePrintf("[%s], after event notify", GetName().c_str());
        return RStatus();
    }
};

class MyPrintEvent: public TaskGraphEvent{
public:
    void Trigger(TaskGraphPassedParam* param) override{
        this_thread::sleep_for(chrono::milliseconds(100));
        auto my_param = GetParamWithNoEmpty<MyParam>("param1");
        ThreadSafePrintf("----> Trigger [%d] times, iValue = [%d]", times_++, my_param->i_value);
    }
private:
    int times_ = 0;

};

int main(){
    MemoryManagerRAII memory_manager_raii;
    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    TaskGraphElement* a = nullptr;
    TaskGraphElement* b = nullptr;
    TaskGraphElement* c = nullptr;
    TaskGraphElement* d = nullptr;

    pipeline->RegisterTaskGraphElement<MyWriteParamNode>(&a, {}, "nodeA");
    pipeline->RegisterTaskGraphElement<MyEventNode>(&b, {a}, "nodeB");
    pipeline->RegisterTaskGraphElement<MyNode1>(&c, {b}, "nodeC");
    pipeline->RegisterTaskGraphElement<MyEventNode>(&d, {b}, "nodeD");

    pipeline->AddTaskGraphEvent<MyPrintEvent>("my-print-event");
    pipeline->Process();

    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
