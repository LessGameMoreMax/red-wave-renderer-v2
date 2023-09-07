#include "LinuxPlatformMemory.h"
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include <iostream>
#endif
#include "../HAL/MallocTC.h"

namespace sablin{

void LinuxPlatformMemory::Initialize(){
    GenericPlatformMemory::Initialize();
}

MallocBase* LinuxPlatformMemory::CreateMallocBase(){
    return new MallocTC();
}

void LinuxPlatformMemory::DebugInfo(){
#ifdef DEBUG
    GenericPlatformMemory::DebugInfo();
    std::cout << "LinuxPLatformMemory" << std::endl;
#endif
}

}
