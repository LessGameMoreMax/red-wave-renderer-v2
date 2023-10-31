#include <iostream>
#include "../../HAL/MemoryManager.h"
#include "../TaskGraphElement/TaskGraphNode/TaskGraphNode.h"
#include "../TaskGraphPipeline/TaskGraphPipeline.h"
#include "../TaskGraphPipeline/TaskGraphPipelineFactory.h"

using namespace sablin;
using namespace std;

class HelloNode: public TaskGraphNode{
public:
    RStatus Run() override{
        std::cout << "Hello! TaskGraph!" << std::endl;
        return RStatus();
    }
};

int main(){
    MemoryManagerRAII memory_manager_raii;
    TaskGraphPipeline* pipeline = TaskGraphPipelineFactory::Create();
    TaskGraphElement* hn = nullptr;
    pipeline->RegisterTaskGraphElement<HelloNode>(&hn);
    RStatus status = pipeline->Process();
    TaskGraphPipelineFactory::Remove(pipeline);
    return 0;
}
