#ifndef TASK_GRAPH_SPIN_LOCK_H
#define TASK_GRAPH_SPIN_LOCK_H
#include <atomic>
namespace sablin{

class TaskGraphSpinLock{
private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
public:
    void Lock(){
        while(flag_.test_and_set(std::memory_order_acquire));
    }

    void UnLock(){
        flag_.clear(std::memory_order_release);
    }

    bool TryLock(){
        return !flag_.test_and_set();
    }
};
}
#endif
