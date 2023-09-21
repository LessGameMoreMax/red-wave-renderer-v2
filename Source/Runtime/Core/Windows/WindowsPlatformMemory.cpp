#include "WindowsPlatformMemory.h"
#include "../HAL/MallocTC.h"
#include "../HAL/MallocAnsi.h"
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include <iostream>
#endif
#include <windows.h>

namespace sablin{

std::map<void*, WindowsPlatformMemory::RawPtrSize> WindowsPlatformMemory::ptr_map_;

void WindowsPlatformMemory::Initialize(){
    GenericPlatformMemory::Initialize();
}

MallocBase* WindowsPlatformMemory::CreateMallocBase(){
#ifdef USE_ANSI_ALLOCTOR
    return new MallocAnsi();
#elif defined(USE_TC_ALLOCTOR)
    return new MallocTC();
#else
#error WindowsPlatorm Have No Memory Alloctor!
#endif
}

void WindowsPlatformMemory::DebugInfo(){
#ifdef DEBUG
    GenericPlatformMemory::DebugInfo();
    std::cout << "WindowsPlatformMemory" << std::endl;
#endif
}

void WindowsPlatformMemory::MemoryOverflow(){
    GenericPlatformMemory::MemoryOverflow();
}

std::pair<void*, std::size_t> WindowsPlatformMemory::BaseMalloc(std::size_t size, std::size_t alignment){
    void* raw_ptr = VirtualAlloc(0, size + alignment, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(raw_ptr == NULL) return {nullptr, 0};
    void* ptr = Align(raw_ptr, alignment);
    std::size_t actual_size = size + alignment - ((uintptr_t)ptr - (uintptr_t)raw_ptr);
    ptr_map_[ptr] = {raw_ptr, size + alignment};
    return {ptr, actual_size};
}

void WindowsPlatformMemory::BaseFree(void* ptr, std::size_t size){
#ifdef DEBUG
    auto iter = ptr_map_.find(ptr);
    ASSERT_WITH_STRING(iter != ptr_map_.end(), "WindowsPlatformMemory::BaseFree: Ptr Is Not In Ptr Map!")
#endif
    ASSERT_WITH_STRING(VirtualFree((iter->second).raw_ptr_, 0, MEM_RELEASE), "WindowsPlatformMemory::BaseFree: Virtual Free Failed!")
    ptr_map_.erase(iter);
}
}
