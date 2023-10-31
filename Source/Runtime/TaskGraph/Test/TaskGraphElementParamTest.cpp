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

struct MyVersionParam: public TaskGraphPassedParam{
public:
    void Clone(TaskGraphPassedParam* param) override{
        auto* ptr = dynamic_cast<MyVersionParam*>(param);
        if(ptr == nullptr) return;
        priority_ = ptr->priority_;
        secondary_ = ptr->secondary_;
    }

    int priority_ = 0;
    int secondary_ = 0;
};

class MyEParamNode: public TaskGraphNode{
public:
    RStatus Run() override{
        auto* version = GetElementParam<MyVersionParam>("version");
        if(version != nullptr) ThreadSafePrintf("[%s] version is [%d-%d]",
                GetName().c_str(), version->priority_, version->secondary_);
        auto* conn = GetElementParam<MyConnParam>("conn");
        if(conn != nullptr) ThreadSafePrintf("[%s] ip = [%s], port = [%u]",
                GetName().c_str(), conn->ip_.c_str(), conn->port_);
        return RStatus();
    }
};

int main(){
    MemoryManagerRAII memory_manager_raii;
    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    TaskGraphElement* a = nullptr;
    TaskGraphElement* v1 = nullptr;
    TaskGraphElement* v2 = nullptr;
    pipeline->RegisterTaskGraphElement<MyNode1>(&a, {}, "node-1");
    pipeline->RegisterTaskGraphElement<MyEParamNode>(&v1, {a}, "version-1");
    pipeline->RegisterTaskGraphElement<MyEParamNode>(&v2, {v1}, "version-2");

    MyVersionParam vp1;
    vp1.priority_ = 1;
    vp1.secondary_ = 1;

    v1->AddElementParam("version", &vp1);

    MyVersionParam vp2;
    vp2.priority_ = 2;
    vp2.secondary_ = 2;

    v2->AddElementParam("version", &vp2);

    MyConnParam cp;
    cp.ip_ = "127.0.0.1";
    cp.port_ = 8080;
    v2->AddElementParam("conn", &cp);

    RStatus status = pipeline->Process();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
