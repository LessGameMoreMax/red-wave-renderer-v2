#include "LinuxPlatformMemory.h"

#if PLATFORM_INDEX == LINUX_PLATFORM_INDEX
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include <iostream>
#endif
#include "../HAL/MallocTC.h"
#include "../HAL/MallocAnsi.h"
#include <sys/mman.h>
#include <unistd.h>

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

std::pair<void*, std::size_t> LinuxPlatformMemory::BaseMalloc(std::size_t size, std::size_t alignment){
    uint32_t linux_page_size = sysconf(_SC_PAGESIZE);
#ifdef DEBUG
    ASSERT_WITH_STRING(alignment % linux_page_size == 0, "LinuxPlatformMemory::BaseMalloc: Size Can Not Be Divided By Linux Page Size!")
#endif
    void* result = mmap(NULL, size + alignment, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(result == NULL) return {nullptr, 0};
    void* left_ptr = Align(result, alignment);
    uintptr_t left_over = (uintptr_t)left_ptr - (uintptr_t)result;
    if(left_over > 0){
        munmap(result, left_over);
        result = left_ptr;
    }
    void* right_ptr = (char*)result + size;
#ifdef DEBUG
    ASSERT_WITH_STRING(alignment >= left_over, "LinuxPlatformMemory::BaseMalloc: Left Over Memory Bigger Than Alignment!")
#endif
    uintptr_t right_over = alignment - left_over;
    if(right_over > 0){
        munmap(right_ptr, right_over);
    }
    return {result, size};
}

void LinuxPlatformMemory::BaseFree(void* ptr, std::size_t size){
    uint32_t linux_page_size = sysconf(_SC_PAGESIZE);
#ifdef DEBUG
    ASSERT_WITH_STRING(size % linux_page_size == 0, "LinuxPlatformMemory::BaseFree: Size Can Not Be Divided By Linux Page Size!")
#endif
    ASSERT_WITH_STRING(munmap(ptr, size) == 0, "LinuxPlatformMemory::BaseFree: Munmap Failed!")
}
}
#endif
