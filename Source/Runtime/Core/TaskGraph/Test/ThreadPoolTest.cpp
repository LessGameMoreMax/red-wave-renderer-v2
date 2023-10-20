#include <iostream>
#include "../../HAL/MemoryManager.h"
#include "../GraphTaskThreadPool.h"

using namespace sablin;
using namespace std;

int main(){
    MemoryManagerRAII memory_manager_raii;
    GraphTaskThreadPool pool;
    pool.Setup();
    return 0;
}

