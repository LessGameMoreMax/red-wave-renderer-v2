#include <iostream>
#include "../TCGlobals.h"
#include "../TCThreadCache.h"
#include <thread>
using namespace sablin;
using namespace std;

void WorkFunction(){
    TCGlobals::thread_local_cache_.Initialize();
    TCGlobals::thread_local_cache_.Clear();
}

int main(){
    TCGlobals::Initialize();
    TCGlobals::thread_local_cache_.Initialize();
    for(int i = 0;i != 10; ++i){
        int *n = new int(1);
        cout << n << endl;
        TCGlobals::thread_local_cache_.GetFreeList(0)->Push(n);
    }
    cout << "------------" << endl;
    while(!TCGlobals::thread_local_cache_.GetFreeList(0)->IsEmpty()){
        cout << TCGlobals::thread_local_cache_.GetFreeList(0)->Pop() << endl;
    }

    cout << "=============" << endl;
    
    for(int i = 0;i != 10; ++i){
        int *n = new int(1);
        cout << n << endl;
        TCGlobals::thread_local_cache_.GetFreeList(0)->Push(n);
    }
    cout << "------------" << endl;
    while(!TCGlobals::thread_local_cache_.GetFreeList(0)->IsEmpty()){
        cout << TCGlobals::thread_local_cache_.GetFreeList(0)->Pop() << endl;
    }
    TCGlobals::thread_local_cache_.Clear();
    TCGlobals::Exit();
}
