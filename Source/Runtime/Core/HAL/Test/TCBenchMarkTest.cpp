#include <iostream>
#include "../GlobalAlloctor.h"
#include "../MemoryManager.h"
#include "../TCGlobals.h"
#include <list>
#include <thread>
#include <vector>
#include <atomic>
#include "../MemoryBase.h"
#include <pthread.h>

using namespace sablin;
using namespace std;
struct Element16{
    int16_t e1;
};

struct Element32{
    int32_t e1;
};

struct Element48{
    int32_t e2;
    int16_t e1;
};

struct Element128{
    int64_t e1;
    int64_t e2;
};

struct Element256{
    Element128 e1;
    Element128 e2;
};

struct Element384{
    Element256 e2;
    Element128 e1;
};

struct Element1024{
    Element256 e1;
    Element256 e2;
    Element256 e3;
    Element256 e4;
};

struct Para{
    size_t times;
    size_t rounds;
};

static atomic<size_t> malloc_time;
static atomic<size_t> free_time;

void* WorkFunc(void* arg){
    size_t times = reinterpret_cast<Para*>(arg)->times;
    size_t rounds = reinterpret_cast<Para*>(arg)->rounds;
    TCGlobals::CreateThreadLocalCache();
    {
        for(size_t j = 0;j != rounds; ++j){
            vector<void*> ptr_vec;
            size_t begin1 = clock();
            for(size_t i = 0;i != times; ++i){
                ptr_vec.push_back(new Element384());
            }
            size_t end1 = clock();
            size_t begin2 = clock();
            for(size_t i = 0;i != times; ++i){
                delete ptr_vec[i];
            }
            size_t end2 = clock();
            malloc_time.fetch_add(end1 - begin1);
            free_time.fetch_add(end2 - begin2);
        }
    }
    TCGlobals::DestroyThreadLocalCache();
    return nullptr;
}


void BenchMarkMalloc(size_t times, size_t works, size_t rounds){
    vector<pthread_t> tid_list;
    vector<Para*> para_list;
    for(size_t k = 0;k != works; ++k){
        tid_list.push_back(0);
        para_list.push_back(new Para{times, rounds});
        pthread_create(&tid_list[k], NULL, WorkFunc, (void*)para_list[k]);
    }

    for(size_t k = 0;k != works; ++k){
        pthread_join(tid_list[k], NULL);
        delete para_list[k];
    }

    cout << works << " threads do " << rounds << " rounds, once malloc " << times << " times, Time: " << malloc_time << " ms" << endl;
    cout << works << " threads do " << rounds << " rounds, once free " << times << " times, Time: " << free_time << " ms" << endl;
    cout << works << " threads do malloc&free " << works * rounds * times << " times, Time: " << malloc_time + free_time << " ms" << endl;
    malloc_time = free_time = 0;
}

int main(){
    MemoryManager::Initialize();
    for(int i = 0;i != 1000; ++i){
        cout << i << endl;
        // BenchMarkMalloc(10000, 10, 10);
        BenchMarkMalloc(1, 16, 1);
    }
    MemoryManager::Exit();
}

