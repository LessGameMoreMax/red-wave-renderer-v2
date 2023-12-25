#include "GenericPlatformMemory.h"
#include "../HAL/MallocAnsi.h"
#include "../Misc/MacroDefine.h"
#include "../Misc/MacroTools.h"
#include "../HAL/Platform.h"
#include <cstring>
namespace sablin{

void GenericPlatformMemory::Initialize(){

}

MallocBase* GenericPlatformMemory::CreateMallocBase(){
    return new MallocAnsi();
}

void GenericPlatformMemory::DebugInfo(){
    //Platform Info
    PlatformInfo::DebugInfo();
}

void GenericPlatformMemory::MemoryOverflow(){
    throw;
}

}
