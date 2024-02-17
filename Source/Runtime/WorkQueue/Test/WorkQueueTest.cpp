#include <iostream>
#include <chrono>
#include "../../Core/HAL/MemoryManager.h"
#include "../WorkQueue.h"

using namespace sablin;
using namespace std;

int Work1(const int number){
    for(int i = 0;i != 100; ++i){
        this_thread::sleep_for(chrono::milliseconds(10));
        std::cout << i << std::endl;
    }
    return number;
}

int main(){
    MemoryManager memory_manager;
    WorkQueue::Initialize();

    auto future1 = WorkQueue::CommitWork(Work1, 2);

    std::cout << future1.Get() << std::endl;

    auto future2 = WorkQueue::CommitWork([](){
        for(int i = 0;i != 100; ++i){
            this_thread::sleep_for(chrono::milliseconds(5));
            std::cout << "--" << i << "--" << std::endl;
        }
    });

    WorkQueue::Exit();
    return 0;
}
