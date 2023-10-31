#include <iostream>
#include "../../Core/HAL/MemoryManager.h"
#include "../TaskGraphElement/TaskGraphNode/TaskGraphNode.h"
#include "../TaskGraphPipeline/TaskGraphPipeline.h"
#include "../TaskGraphPipeline/TaskGraphPipelineFactory.h"
#include "../../Core/Utility/ThreadSafeIostream.h"

using namespace sablin;
using namespace std;

template<typename... Args>
class MyTemplateNode: public TaskGraphTemplateNode<Args...>{
public:
    explicit MyTemplateNode(int num, float score){
        num_ = num;
        score_ = score;
    }
    explicit MyTemplateNode(int num){
        num_ = num;
        score_ = 7.0f;
    }
    RStatus Run() override{
        ThreadSafePrintf("[MyTemplateNode] num = [%d], score = [%f]", num_, score_);
        return RStatus();
    }
private:
    int num_;
    float score_;
};

int main(){
    MemoryManagerRAII memory_manager_raii;
    RStatus status;
    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    TaskGraphTemplateNode<int, float>* a = nullptr;
    TaskGraphTemplateNode<int, float>* b = nullptr;
    TaskGraphTemplateNode<int>* c = nullptr;

    pipeline->RegisterTaskGraphElement<MyTemplateNode<int, float>>(&a, {}, 3, 3.5f);
    pipeline->RegisterTaskGraphElement<MyTemplateNode<int, float>>(&b, {a}, 5, 3.75f);
    pipeline->RegisterTaskGraphElement<MyTemplateNode<int>>(&c, {b}, 8);
    pipeline->Process();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
