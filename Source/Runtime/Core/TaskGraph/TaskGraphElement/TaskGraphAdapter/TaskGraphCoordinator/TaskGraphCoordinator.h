#ifndef TASK_GRAPH_COORDINATOR_H
#define TASK_GRAPH_COORDINATOR_H
#include "../TaskGraphAdapter.h"
namespace sablin{

template<int32_t SIZE>
class TaskGraphCoordinator: public TaskGraphAdapter{
protected:
    explicit TaskGraphCoordinator():
        TaskGraphAdapter(){
        element_type_ = TaskGraphElementType::kCoordinator;
        session_ = TaskGraphRandom<>::GenerateSession("coordinator");
    }

    virtual RStatus Run() final{
        return SIZE >= 0? thread_pool_->CreateSecondaryThread(SIZE):
            thread_pool_->ReleaseSecondaryThread((-1)*SIZE);
    }

    virtual void DumpElement(std::ostream& oss) final{
        DumpElementHeader(oss);
        DumpPerfInfo(oss);
        oss << "\", shape=diamond]\n";
    }

    virtual RStatus CheckSuitable() final{
        if(loop_>1) return RStatus("Can Not Set TaskGraphCoordinator Loop > 1!");
        return RStatus();
    }
};


}
#endif
