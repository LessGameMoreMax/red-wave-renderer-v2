#include <iostream>
#include "../../Core/HAL/GlobalAlloctor.h"
#include "../../Core/HAL/MemoryManager.h"
#include "../../Core/HAL/TCGlobals.h"
#include <list>
#include <thread>
#include <vector>
#include <atomic>
#include "../../Core/HAL/MemoryBase.h"
#include "../../Core/HAL/Runnable.h"
#include "../../Core/GenericPlatform/GenericPlatformProcess.h"

using namespace sablin;
using namespace std;

class MyRunnable: public Runnable{
public:
    MyRunnable() = default;
    virtual ~MyRunnable() = default;

    virtual RStatus Run() override{
        while(1){
            std::cout << "1" << std::endl;
	    this_thread::sleep_for(chrono::seconds(1));
        }
        return RStatus();
    }
};

int main(){
    MemoryManagerRAII memory_manager_raii;
    MyRunnable* runnable = new MyRunnable();
    RunnableThread* runnable_thread = PlatformProcess::CreateNativeThread(runnable);
    CpuSet cpu_set;
    cpu_set.cpu_set_.insert(2);
    runnable_thread->SetThreadAffinity(cpu_set);

    while(1){
    this_thread::sleep_for(chrono::seconds(3));
        runnable_thread->Suspend(true);
        std::cout << "Stop!" << std::endl;
        this_thread::sleep_for(chrono::seconds(5));
        runnable_thread->Suspend(false);
    }
    if(runnable_thread->IsJoinable()){
        runnable_thread->Join();
    }
    delete runnable_thread;
    delete runnable;
}

