#include <iostream>
#include "../../HAL/GlobalAlloctor.h"
#include "../../HAL/MemoryManager.h"
#include "../../HAL/TCGlobals.h"
#include <list>
#include <thread>
#include <vector>
#include <atomic>
#include "../../HAL/MemoryBase.h"
#include "../Runnable.h"
#include "../../GenericPlatform/GenericPlatformProcess.h"

using namespace sablin;
using namespace std;

class MyRunnable: public Runnable{
public:
    MyRunnable() = default;
    virtual ~MyRunnable() = default;

    virtual RStatus Run() override{
        while(1){
            std::cout << "1" << std::endl;
        }
        return RStatus();
    }
};

int main(){
    MemoryManager::Initialize();
    {
        MyRunnable* runnable = new MyRunnable();
        RunnableThread* runnable_thread = PlatformProcess::CreateThread(runnable);
        CpuSet cpu_set;
        cpu_set.cpu_set_.insert(2);
        runnable_thread->SetThreadAffinity(cpu_set);
        while(1){
            runnable_thread->Suspend(true);
            std::cout << "Stop!" << std::endl;
            this_thread::sleep_for(chrono::seconds(10));
            runnable_thread->Suspend(false);
        }
        if(runnable_thread->IsJoinable()){
            runnable_thread->Join();
        }
        delete runnable_thread;
        delete runnable;
    }
    MemoryManager::Exit();
}

