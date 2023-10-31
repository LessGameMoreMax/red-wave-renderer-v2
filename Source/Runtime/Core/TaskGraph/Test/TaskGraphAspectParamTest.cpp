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

class MyTimerAspect: public TaskGraphAspect{
public:
    RStatus BeginRun() override{
        start_ts_ = std::chrono::steady_clock::now();
        return RStatus();
    }

    void FinishRun(const RStatus& cur_status) override{
        std::chrono::duration<double, std::milli> span = std::chrono::steady_clock::now() - start_ts_;
        ThreadSafePrintf("---> [MyTimerAspect] [%s] time cost is : [%0.2lf] ms", GetName().c_str(), span.count());
    }
private:
    std::chrono::steady_clock::time_point start_ts_;
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

class MyConnAspect: public TaskGraphAspect{
private:
    bool conn_status_ = false;
protected:
    void MockConnect(const std::string& ip, short port){
        ThreadSafePrintf("----> [MyConnAspect] [%s : %d] has been connected ...", ip.c_str(), port);
        conn_status_ = true;
    }

    void MockDisconnect(const std::string& ip, short port){
        if(conn_status_){
            ThreadSafePrintf("----> [MyConnAspect] [%s : %d] has been disconnected ...", ip.c_str(), port);
            conn_status_ = false;
        }else{
            ThreadSafePrintf("----> [MyConnAspect] [%s : %d] is already been disconnected ...", ip.c_str(), port);
        }
    }
public:
    RStatus BeginInit() override{
        auto* param = this->GetAspectParam<MyConnParam>();
        if(param) MockConnect(param->ip_, param->port_);
        return RStatus();
    }

    void FinishDestroy(const RStatus& cur_status) override{
        auto* param = this->GetAspectParam<MyConnParam>();
        if(param) MockDisconnect(param->ip_, param->port_);
    }
};

class MyPipelineParamAspect: public TaskGraphAspect{
public:
    RStatus BeginRun() override{
        RStatus status;
        auto* pipeline_param = GetParamWithNoEmpty<MyParam>("param1");
        int cut = pipeline_param->i_count;
        ThreadSafePrintf("-----> [MyPipelineParamAspect] pipeline param iCount is [%d] before run.", cut);
        if(cut < 0) return RStatus("Aspect demo error");
        return RStatus();
    }
};

int main(){
    MemoryManagerRAII memory_manager_raii;
    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    MyConnParam param_a;
    param_a.ip_ = "127.0.0.1";
    param_a.port_ = 6666;

    MyConnParam param_b;
    param_b.ip_ = "255.255.255.255";
    param_b.port_ = 9999;

    TaskGraphElement* a = nullptr;
    TaskGraphElement* b = nullptr;
    TaskGraphElement* c = nullptr;

    pipeline->RegisterTaskGraphElement<MyNode1>(&a, {}, "nodeA", 1);
    pipeline->RegisterTaskGraphElement<MyNode2>(&b, {a}, "nodeB", 1);
    pipeline->RegisterTaskGraphElement<MyWriteParamNode>(&c, {b}, "nodeC", 2);

    a->AddTaskGraphAspect<MyConnAspect, MyConnParam>(&param_a);
    b->AddTaskGraphAspect<MyConnAspect, MyConnParam>(&param_b)->AddTaskGraphAspect<MyTimerAspect>();
    c->AddTaskGraphAspect<MyPipelineParamAspect>();
    RStatus status = pipeline->Process();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
