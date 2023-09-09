#include "GenericPlatformMemory.h"
#include "../HAL/MallocAnsi.h"
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include "../Misc/MacroTools.h"
#include "../HAL/Platform.h"
#endif
#include <cstring>
namespace sablin{

void GenericPlatformMemory::Initialize(){

}

MallocBase* GenericPlatformMemory::CreateMallocBase(){
    return new MallocAnsi();
}

void GenericPlatformMemory::DebugInfo(){
#ifdef DEBUG
    //Platform Info
    PlatformInfo::DebugInfo();
#endif
}

void GenericPlatformMemory::MemoryOverflow(){
    throw;
}

void GenericPlatformMemory::Memcpy(void* dest, const void* src, std::size_t count){
    std::memcpy(dest, src, count);
}

}
