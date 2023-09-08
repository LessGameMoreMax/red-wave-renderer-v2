#include "LinuxPlatformMemory.h"
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include <iostream>
#endif
#include "../HAL/MallocTC.h"
#include "../HAL/MallocAnsi.h"

namespace sablin{

void LinuxPlatformMemory::Initialize(){
    GenericPlatformMemory::Initialize();
}

MallocBase* LinuxPlatformMemory::CreateMallocBase(){
#ifdef USE_ANSI_ALLOCTOR
    return new MallocAnsi();
#elif defined(USE_TC_ALLOCTOR)
    return new MallocTC();
#else
#error LinuxPlatform Have No Memory Alloctor!
#endif
}

void LinuxPlatformMemory::DebugInfo(){
#ifdef DEBUG
    GenericPlatformMemory::DebugInfo();
    std::cout << "LinuxPLatformMemory" << std::endl;
#endif
}

void LinuxPlatformMemory::MemoryOverflow(){
    GenericPlatformMemory::MemoryOverflow();
}
}
