#include <iostream>
#include "../MemoryManager.h"
#include "../MemoryBase.h"
#include "../GlobalAlloctor.h"

using namespace sablin;
using namespace std;

int main(){
    MemoryManager::Initialize();
    MemoryBase::DebugInfo();
    int* i = new int(1);
    delete i;
    int* j = new int(4);
    delete j;
    string* s = new string("hello");
    delete s;
    MemoryManager::Exit();
    return 0;
}
