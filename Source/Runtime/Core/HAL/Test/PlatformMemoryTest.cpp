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
    int* i = new int(1);
    delete i;
    // int* j = new int(4);
    // delete j;
    // string* s = new string("hello");
    // delete s;
    // TCSizeMap *size_map = new TCSizeMap();
    // cout << size_map->Initialize() << endl;
    // delete size_map;
    MemoryManager::Exit();

    return 0;
}
