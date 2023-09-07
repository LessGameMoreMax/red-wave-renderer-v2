#include "WindowsPlatformMemory.h"
#include "../HAL/MallocTC.h"

namespace sablin{

void WindowsPlatformMemory::Initialize(){
    GenericPlatformMemory::Initialize();
}

MallocBase* WindowsPlatformMemory::CreateMallocBase(){
    return new MallocTC();
}

void WindowsPlatformMemory::DebugInfo(){
#ifdef DEBUG
    GenericPlatformMemory::DebugInfo();
    std::cout << "WindowsPlatformMemory" << std::endl;
#endif
}

}
