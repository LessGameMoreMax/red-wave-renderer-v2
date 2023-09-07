#include "GenericPlatformMemory.h"
#include "../HAL/MallocAnsi.h"
#ifdef DEBUG
#include "../Misc/MacroTools.h"
#include "../HAL/Platform.h"
#endif
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

}
