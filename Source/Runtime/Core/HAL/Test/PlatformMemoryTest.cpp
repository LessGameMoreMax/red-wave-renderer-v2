#include <iostream>
#include "../MemoryManager.h"
#include "../MemoryBase.h"
#include "../GlobalAlloctor.h"
#include "../TCSizeMap.h"

using namespace sablin;
using namespace std;

int main(){
    MemoryManager::Initialize();
    MemoryBase::DebugInfo();
    for(uint64_t i = (1 << 20);i != (1 << 31); ++i){
        std::cout << i << std::endl;
        char* c = new char[i];
        delete[] c;
    }

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
    char* c = new char[1409025];
    delete[] c;
    MemoryManager::Exit();

    return 0;
}
