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
    {
        MemoryBase::DebugInfo();
        // uint64_t result = (1 << 18) + (1 << 10);
        // uint64_t result = (1 << 18);
        uint64_t result = 1 << 20;
        // void* temp[result];
        list<char*> lt;
        for(uint64_t i = 0;i != result; ++i){
            std::cout << i << std::endl;
            char* c = new char[i];
            // temp[i] = c;
            lt.push_front(c);
        }
        for(uint64_t i = 0;i != result; ++i){
            std::cout << i << std::endl;
            char* c = lt.front();
            lt.pop_front();
            delete[] c;
        }
        std::cout << "--------" << std::endl;
    }
    MemoryManager::Exit();

    return 0;
}
