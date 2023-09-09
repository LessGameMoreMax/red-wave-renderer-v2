#include "MallocAnsi.h"
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include <iostream>
#endif
#include "PlatformMemory.h"
#include "../Math/MathTools.h"
namespace sablin{

MallocAnsi::MallocAnsi():
        MallocBase()
{

}

MallocAnsi::~MallocAnsi(){

}

void* MallocAnsi::Malloc(std::size_t size, uint32_t alignment){
#ifdef DEBUG
    std::cout << "MallocAnsi::Malloc" << std::endl;
#endif
    void* ptr = TryMalloc(size, alignment);
    if(ptr == nullptr)[[unlikely]] PlatformMemory::MemoryOverflow();
    return ptr;
}

void* MallocAnsi::TryMalloc(std::size_t size, uint32_t alignment){
    if(size < 16){
        alignment = Max<uint32_t>(8, alignment);
    }else{
        alignment = Max<uint32_t>(16, alignment);
    }
}

void* MallocAnsi::Realloc(void* ptr, std::size_t new_size, uint32_t alignment){
    return nullptr;
}

void* MallocAnsi::TryRealloc(void* ptr, std::size_t new_size, uint32_t alignment){
    return nullptr;
}

void MallocAnsi::Free(void* ptr){
    if(ptr == nullptr)[[unlikely]] return;
    ::free(ptr); 
}

std::string MallocAnsi::GetDescriptName() const{
    return "Ansi";
}

bool MallocAnsi::IsThreadSafe() const{
    return true;
}

}
