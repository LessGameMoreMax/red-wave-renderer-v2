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

static atomic<size_t> malloc_time;
static atomic<size_t> free_time;

class MyRunnable: public Runnable{
public:
    size_t times;
    size_t rounds;

public:
    MyRunnable() = default;
    virtual ~MyRunnable() = default;

    virtual RStatus Run() override{
        for(size_t j = 0;j != rounds; ++j){
            vector<char*> ptr_vec;
            size_t begin1 = clock();
            for(size_t i = 0;i != times; ++i){
                ptr_vec.push_back(new char[1024]);
            }
            size_t end1 = clock();
            size_t begin2 = clock();
            for(size_t i = 0;i != times; ++i){
                delete[] ptr_vec[i];
            }
            size_t end2 = clock();
            malloc_time.fetch_add(end1 - begin1);
            free_time.fetch_add(end2 - begin2);
        }
        return RStatus();
    }
};


void BenchMarkMalloc(size_t times, size_t works, size_t rounds){
    vector<RunnableThread*> thread_list;
        
    MyRunnable* runnable = new MyRunnable();
    runnable->times = times;
    runnable->rounds = rounds;
    for(size_t k = 0;k != works; ++k){
        RunnableThread* runnable_thread = PlatformProcess::CreateNativeThread(runnable);
        thread_list.push_back(runnable_thread);
    }

    for(size_t k = 0;k != works; ++k){
        if(thread_list[k]->IsJoinable()){
            thread_list[k]->Join();
        }
        delete thread_list[k];
    }

    delete runnable;
    cout << works << " threads do " << rounds << " rounds, once malloc " << times << " times, Time: " << malloc_time << " ms" << endl;
    cout << works << " threads do " << rounds << " rounds, once free " << times << " times, Time: " << free_time << " ms" << endl;
    cout << works << " threads do malloc&free " << works * rounds * times << " times, Time: " << malloc_time + free_time << " ms" << endl;
    malloc_time = free_time = 0;
}

int main(){
    MemoryManager::Initialize();
    for(int i = 0;i != 1000; ++i){
        cout << i << endl;
        BenchMarkMalloc(1000, 8, 10);
    }
    MemoryManager::Exit();
}

