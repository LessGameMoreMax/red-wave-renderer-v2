#include <iostream>
#include "../MemoryManager.h"
#include "../MemoryBase.h"
#include "../GlobalAlloctor.h"

using namespace sablin;
using namespace std;

int main(){
    MemoryManager::Initialize();
    MemoryBase::DebugInfo();
    int* i = new int();
    // delete i;
    // if(i == nullptr) cout << "i is nullptr" << endl;
    MemoryManager::Exit();
    return 0;
}
