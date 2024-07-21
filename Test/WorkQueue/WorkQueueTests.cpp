#include <iostream>
#include <chrono>
#include "Core/Memory/MemoryManager.h"
#include "WorkQueue/WorkQueue.h"

using namespace sablin;
using namespace std;

class MyWorkRunnable: public WorkRunnable<int, int>{
public:
    explicit MyWorkRunnable(int number): WorkRunnable(number){}

    virtual int Work(int number) override{
        for(int i = 0;i != 100; ++i){
            this_thread::sleep_for(chrono::milliseconds(10));
            std::cout << i << std::endl;
        }
        return number;
    }
};

int main(){
    MemoryManager memory_manager;
    WorkQueue::Initialize();

    auto future1 = WorkQueue::CommitWork<MyWorkRunnable>(2);

    cout << future1.Get() << endl;

    auto future2 = WorkQueue::CommitWork([](){
        for(int i = 0;i != 100; ++i){
            this_thread::sleep_for(chrono::milliseconds(5));
            std::cout << "--" << i << "--" << std::endl;
        }
    });

    WorkQueue::Exit();
    return 0;
}
