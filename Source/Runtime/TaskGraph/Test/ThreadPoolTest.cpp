#include <iostream>
#include "../../Core/HAL/MemoryManager.h"
#include "../GraphTaskThreadPool.h"

using namespace sablin;
using namespace std;

int main(){
    MemoryManagerRAII memory_manager_raii;
    GraphTaskThreadPool pool;
    pool.Setup();
    this_thread::sleep_for(chrono::seconds(10));
    return 0;
}

