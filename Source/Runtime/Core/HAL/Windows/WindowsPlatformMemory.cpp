#include "WindowsPlatformMemory.h"
#if PLATFORM_INDEX == WINDOWS_PLATFORM_INDEX
#include "Core/Memory/MallocTC.h"
#include "Core/Memory/MallocAnsi.h"
#include "Macro/MacroDefine.h"
#ifdef DEBUG
#include <iostream>
#endif

#include <windows.h>

namespace sablin{

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
    void* raw_ptr = VirtualAlloc(0, sizeof(std::size_t) + sizeof(void*) + size + alignment, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(raw_ptr == NULL) return {nullptr, 0};
    void* ptr = Align((uint8_t*)raw_ptr + sizeof(std::size_t) + sizeof(void*), alignment);
    *(void**)((uint8_t*)ptr - sizeof(void*)) = raw_ptr;
    *(std::size_t*)((uint8_t*)ptr - sizeof(void*) - sizeof(std::size_t)) = size;
    std::size_t actual_size = size + alignment - ((uintptr_t)ptr - (uintptr_t)raw_ptr) + sizeof(std::size_t) + sizeof(void*);
    return {ptr, actual_size};
}

void WindowsPlatformMemory::BaseFree(void* ptr, std::size_t size){
    ASSERT_WITH_STRING(VirtualFree(*(void**)((uint8_t*)ptr - sizeof(void*)), 0, MEM_RELEASE), "WindowsPlatformMemory::BaseFree: Virtual Free Failed!")
}
}
#endif
