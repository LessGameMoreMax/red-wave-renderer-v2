#include <iostream>
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/ThreadLocalMemory.h"
#include <list>
#include <thread>
#include <vector>
#include <atomic>
#include <Windows.h>

using namespace sablin;
using namespace std;

struct Para{
    size_t times;
    size_t rounds;
};

static atomic<size_t> malloc_time;
static atomic<size_t> free_time;

DWORD WINAPI WorkFunc(LPVOID arg){
    size_t times = reinterpret_cast<Para*>(arg)->times;
    size_t rounds = reinterpret_cast<Para*>(arg)->rounds;
    ThreadLocalMemory thread_local_memory;
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
    return 0;
}


void BenchMarkMalloc(size_t times, size_t works, size_t rounds){
    HANDLE handle_list[works];
    vector<Para*> para_list;
    for(size_t k = 0;k != works; ++k){
        Para* temp = new Para{times, rounds};
        assert(temp != nullptr);
        para_list.push_back(temp);
	    handle_list[k] = CreateThread(NULL, 0, WorkFunc, (LPVOID)temp, 0, NULL);
    }

    WaitForMultipleObjects(works, handle_list, TRUE, INFINITE);
    for(size_t k = 0;k != works; ++k){
	    CloseHandle(handle_list[k]);
    }
    cout << works << " threads do " << rounds << " rounds, once malloc " << times << " times, Time: " << malloc_time << " ms" << endl;
    cout << works << " threads do " << rounds << " rounds, once free " << times << " times, Time: " << free_time << " ms" << endl;
    cout << works << " threads do malloc&free " << works * rounds * times << " times, Time: " << malloc_time + free_time << " ms" << endl;
    malloc_time = free_time = 0;
}

int main(){
    MemoryManager memory_manager;
   for(int i = 0;i != 1000; ++i){
       cout << i << endl;
       BenchMarkMalloc(5000, 8, 10);
   }
}

