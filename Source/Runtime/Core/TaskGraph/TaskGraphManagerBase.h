#ifndef TASK_GRAPH_MANAGER_BASE_H 
#define TASK_GRAPH_MANAGER_BASE_H
#include "../HAL/RStatus.h"
#include "../Debug/Assertion.h"
namespace sablin{

template <typename T>
class TaskGraphManagerBase{
protected:
    virtual ~TaskGraphManagerBase() = default;

    virtual RStatus Add(T* object){
        NO_IMPLEMENT_ASSERT(TaskGraphManagerBase, Add)
    }

    virtual RStatus Remove(T* object){
        NO_IMPLEMENT_ASSERT(TaskGraphManagerBase, Remove)
    }

    virtual bool Find(T* object) const{
        return false;
    }

    virtual RStatus Create(const std::string& key){
        NO_IMPLEMENT_ASSERT(TaskGraphManagerBase, Create)
    }

    virtual T* Get(const std::string& key){
        return nullptr;
    }

    virtual RStatus Reset(){
        NO_IMPLEMENT_ASSERT(TaskGraphManagerBase, Reset)
    }

    virtual size_t GetSize() const{
        return 0;
    }

    virtual RStatus Clear() = 0;
};

}
#endif
