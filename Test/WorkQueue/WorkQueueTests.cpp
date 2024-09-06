#include <iostream>
#include <chrono>
#include "Core/Memory/MemoryManager.h"
#include "WorkQueue/WorkQueue.h"
#include "Utility/ThreadSafeIostream.h"

using namespace sablin;
using namespace std;

class MyWorkRunnable: public WorkRunnable<int, int>{
public:
    explicit MyWorkRunnable(int number): WorkRunnable(number){}

    virtual int Work(int number) override{
        // ThreadSafePrintf("good!");
        // for(int i = 0;i != 20; ++i){
            // this_thread::sleep_for(chrono::milliseconds(10));
            // std::cout << i << std::endl;
        // }
        std::cout << number << std::endl;
        return number;
    }
};

int main(){
    MemoryManager memory_manager;
    WorkQueue::Initialize();

    // std::vector<WorkSharedFuture<int>> v;
    // for(int i = 0;i != 50; ++i){
    //     auto future1 = WorkQueue::CommitWork<MyWorkRunnable>(2);
    //     v.push_back(std::move(future1));
    // }
    // ThreadSafePrintf("good!");
    // auto future1 = WorkQueue::CommitTimerWork<MyWorkRunnable>(2, 1000, 2);
    // auto future2 = WorkQueue::CommitWork([](){
    //     for(int i = 0;i != 100; ++i){
    //         this_thread::sleep_for(chrono::milliseconds(5));
    //         std::cout << "--" << i << "--" << std::endl;
    //     }
    // });
    // std::cout << future1.Get() << std::endl;
    // future2.Get();
    // for(auto f: v) f.Get();
    auto p = WorkQueue::CommitTimerWork<MyWorkRunnable>(10, 500, 2);
    std::this_thread::sleep_for(chrono::seconds(2));
    p.second->Stop();
    std::this_thread::sleep_for(chrono::seconds(2));

    WorkQueue::Exit();
    return 0;
}
