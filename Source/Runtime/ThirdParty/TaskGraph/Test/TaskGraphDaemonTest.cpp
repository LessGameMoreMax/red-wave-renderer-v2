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
#include "../TaskGraphDaemon/TaskGraphDaemon.h"
#include "../TaskGraphDaemon/TaskGraphTemplateDaemon.h"

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

struct MyConnParam: public TaskGraphPassedParam{
    void Clone(TaskGraphPassedParam* param) override{
        if(param == nullptr) return;
        auto* ptr = dynamic_cast<MyConnParam*>(param);
        ip_ = ptr->ip_;
        port_ = ptr->port_;
    }
    std::string ip_{"0.0.0.0"};
    short port_{0};
};

class MyMonitorDaemon: public TaskGraphDaemon{
public:
    void DaemonTask(TaskGraphPassedParam* param) override{
        ThreadSafePrintf("----> [MyMonitorDaemon] this monitor is still running, monitor span is [%ld] ms....", GetInterval());
    }
};

class MyParamDaemon: public TaskGraphDaemon{
public:
    void DaemonTask(TaskGraphPassedParam* param) override{
        auto* my_param = GetParamWithNoEmpty<MyParam>("param1");
        ThreadSafePrintf("----> [MyParamDaemon] iCount is [%d], just suppose this value is show on dashboard every [%ld] ms", my_param->i_count, GetInterval());

        auto* conn_param = dynamic_cast<MyConnParam*>(param);
        if(conn_param){
            ThreadSafePrintf("----> [MyParamDaemon] input param, address is [%s: %d]",conn_param->ip_.c_str(), conn_param->port_);
        }else{
            ThreadSafePrintf("----> [MyParamDaemon] no input param info.");
        }
    }

};

template<class... Args>
class MyTemplateDaemon: public TaskGraphTemplateDaemon<Args...>{
    int index_ = 0;
public:
    explicit MyTemplateDaemon(int index){
        index_ = index;
    }

    void DaemonTask(TaskGraphPassedParam* param) override{
        ThreadSafePrintf("----> [MyTemplateDaemon] template input index is [%d]", index_);
    }

};

int main(){
    MemoryManagerRAII memory_manager_raii;
    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    TaskGraphElement* a = nullptr;
    TaskGraphElement* b = nullptr;

    pipeline->RegisterTaskGraphElement<MyNode1>(&a, {}, "nodeA");
    pipeline->RegisterTaskGraphElement<MyWriteParamNode>(&b, {a}, "WriteParamNodeB");

    MyConnParam conn_param;
    conn_param.ip_ = "127.0.0.1";
    conn_param.port_ = 6666;

    pipeline->AddTaskGraphDaemon<MyMonitorDaemon>(4000)->AddTaskGraphDaemon<MyParamDaemon, MyConnParam>(3500, &conn_param)->AddTaskGraphDaemon<MyTemplateDaemon<int>>(2750, 300);

    RStatus status = pipeline->Process(20);
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
