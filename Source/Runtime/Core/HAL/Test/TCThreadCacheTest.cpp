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
    for(int i = 0;i != 4; ++i){
        thread t(WorkFunction);
        t.join();
    }
    TCGlobals::thread_local_cache_.Clear();
    TCGlobals::Exit();
}
