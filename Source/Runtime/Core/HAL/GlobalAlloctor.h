#ifndef GLOBAL_ALLOCTOR_H
#define GLOBAL_ALLOCTOR_H
#include "MemoryBase.h"
#include "Platform.h"
#include "../Misc/MacroDefine.h"
#include INCLUDE_PLATFORM_HEADER(.., Platform.h)

void* operator new(std::size_t size){
#ifdef DEBUG
    std::cout << "operator new(std::size_t size)" << std::endl;
#endif
    return sablin::MemoryBase::Malloc(size, MEMORY_ALIGNMENT_DEFAULT);
}

void operator delete(void* ptr){
#ifdef DEBUG
    std::cout << "operator delete(void* ptr)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}
#endif
