#include <iostream>
#include <chrono>
#include "../../Core/HAL/MemoryManager.h"
#include "../WorkQueue.h"

using namespace sablin;
using namespace std;

int main(){
    MemoryManager memory_manager;
    WorkQueue::Initialize();
    for(int i = 0;i != 100; ++i)
        this_thread::sleep_for(chrono::milliseconds(10));
    WorkQueue::Exit();
    return 0;
}
