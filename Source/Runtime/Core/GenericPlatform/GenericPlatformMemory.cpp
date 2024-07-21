#include "GenericPlatformMemory.h"
#include "Core/Memory/MallocAnsi.h"
#include "Macro/MacroDefine.h"
#include "Macro/MacroTools.h"
#include "Macro/Platform.h"
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
