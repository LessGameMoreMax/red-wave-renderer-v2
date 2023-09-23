#include <iostream>
#include "../MemoryManager.h"
#include "../MemoryBase.h"
#include "../GlobalAlloctor.h"
#include "../TCSizeMap.h"
#include <list>

using namespace sablin;
using namespace std;

int main(){
    MemoryManager::Initialize();
    MemoryBase::DebugInfo();
    uint64_t result = 1 << 18;
    list<char*> lt;
    for(uint64_t i = 0;i != result; ++i){
        std::cout << i << std::endl;
        char* c = new char[i];
        lt.push_front(c);
    }
    for(uint64_t i = 0;i != result; ++i){
        std::cout << i << std::endl;
        char* c = lt.front();
        lt.pop_front();
        delete[] c;
    }
    std::cout << "--------" << std::endl;

    // int* j = new int(0);
    // // std::cout << (void*)j << std::endl;
    // delete j;
    // std::cout << "-------" << std::endl;
    // string* s = new string("hello");
    // // cout << (void*)s << endl;
    // delete s;

    // std::cout << "----------" << std::endl;

    // TCSizeMap *size_map = new TCSizeMap();
    // cout << size_map->Initialize() << endl;
    // delete size_map;
    
    MemoryManager::Exit();

    return 0;
}
