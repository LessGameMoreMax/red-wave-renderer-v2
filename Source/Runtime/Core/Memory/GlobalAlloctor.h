#ifndef GLOBAL_ALLOCTOR_H
#define GLOBAL_ALLOCTOR_H
#include "MemoryBase.h"
#include "Macro/Platform.h"
#include "Macro/MacroDefine.h"
#include INCLUDE_PLATFORM_HEADER(Platform.h)

void* operator new(std::size_t size){
#ifdef DEBUG
    std::cout << "operator new(std::size_t size)" << std::endl;
#endif
    if(size == 0) [[unlikely]]{
        size = 1;
    }
#if !defined(USE_PLATFORM_ALIGNMENT) && __STDCPP_DEFAULT_NEW_ALIGNMENT__ <= 8u
    return sablin::MemoryBase::Malloc(size, MEMORY_MIN_ALIGNMENT);
#else
    return sablin::MemoryBase::Malloc(size, MEMORY_ALIGNMENT_DEFAULT);
#endif
}

void* operator new[](std::size_t size){
#ifdef DEBUG
    std::cout << "operator new[](std::size_t size)" << std::endl;
#endif
    if(size == 0) [[unlikely]]{
        size = 1;
    }
#if !defined(USE_PLATFORM_ALIGNMENT) && __STDCPP_DEFAULT_NEW_ALIGNMENT__ <= 8u
    return sablin::MemoryBase::Malloc(size, MEMORY_MIN_ALIGNMENT);
#else
    return sablin::MemoryBase::Malloc(size, MEMORY_ALIGNMENT_DEFAULT);
#endif
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator new(std::size_t size, const std::nothrow_t&)" << std::endl;
#endif
    if(size == 0) [[unlikely]]{
        size = 1;
    }
#if !defined(USE_PLATFORM_ALIGNMENT) && __STDCPP_DEFAULT_NEW_ALIGNMENT__ <= 8u
    return sablin::MemoryBase::Malloc(size, MEMORY_MIN_ALIGNMENT);
#else
    return sablin::MemoryBase::Malloc(size, MEMORY_ALIGNMENT_DEFAULT);
#endif
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator new[](std::size_t size, const std::nothrow_t&)" << std::endl;
#endif
    if(size == 0) [[unlikely]]{
        size = 1;
    }
#if !defined(USE_PLATFORM_ALIGNMENT) && __STDCPP_DEFAULT_NEW_ALIGNMENT__ <= 8u
    return sablin::MemoryBase::Malloc(size, MEMORY_MIN_ALIGNMENT);
#else
    return sablin::MemoryBase::Malloc(size, MEMORY_ALIGNMENT_DEFAULT);
#endif
}
//------------------------

void* operator new(std::size_t size, uint32_t alignment){
#ifdef DEBUG
    std::cout << "operator new(std::size_t size, uint32_t alignment)" << std::endl;
#endif
    if(size == 0) [[unlikely]]{
        size = 1;
    }
    return sablin::MemoryBase::Malloc(size, alignment);
}

void* operator new[](std::size_t size, uint32_t alignment){
#ifdef DEBUG
    std::cout << "operator new[](std::size_t size, uint32_t alignment)" << std::endl;
#endif
    if(size == 0) [[unlikely]]{
        size = 1;
    }
    return sablin::MemoryBase::Malloc(size, alignment);
}

void* operator new(std::size_t size, uint32_t alignment, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator new(std::size_t size, uint32_t alignment, const std::nothrow_t&)" << std::endl;
#endif
    if(size == 0) [[unlikely]]{
        size = 1;
    }
    return sablin::MemoryBase::Malloc(size, alignment);
}

void* operator new[](std::size_t size, uint32_t alignment, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator new[](std::size_t size, uint32_t alignment, const std::nothrow_t&)" << std::endl;
#endif
    if(size == 0) [[unlikely]]{
        size = 1;
    }
    return sablin::MemoryBase::Malloc(size, alignment);
}
//------------------------
void operator delete(void* ptr){
#ifdef DEBUG
    std::cout << "operator delete(void* ptr)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete[](void* ptr){
#ifdef DEBUG
    std::cout << "operator delete[](void* ptr)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator delete(void* ptr, const std::nothrow_t&)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator delete[](void* ptr, const std::nothrow_t&)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}
//---------------------------
void operator delete(void* ptr, std::size_t size){
#ifdef DEBUG
    std::cout << "operator delete(void* ptr)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete[](void* ptr, std::size_t size){
#ifdef DEBUG
    std::cout << "operator delete[](void* ptr)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete(void* ptr, std::size_t size, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator delete(void* ptr, const std::nothrow_t&)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete[](void* ptr, std::size_t size, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator delete[](void* ptr, const std::nothrow_t&)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}
//---------------------------
void operator delete(void* ptr, std::size_t size, uint32_t alignment){
#ifdef DEBUG
    std::cout << "operator delete(void* ptr)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete[](void* ptr, std::size_t size, uint32_t alignment){
#ifdef DEBUG
    std::cout << "operator delete[](void* ptr)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete(void* ptr, std::size_t size, uint32_t alignment, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator delete(void* ptr, const std::nothrow_t&)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete[](void* ptr, std::size_t size, uint32_t alignment, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator delete[](void* ptr, const std::nothrow_t&)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}
//---------------------------
void operator delete(void* ptr, uint32_t alignment){
#ifdef DEBUG
    std::cout << "operator delete(void* ptr)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete[](void* ptr, uint32_t alignment){
#ifdef DEBUG
    std::cout << "operator delete[](void* ptr)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete(void* ptr, uint32_t alignment, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator delete(void* ptr, const std::nothrow_t&)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}

void operator delete[](void* ptr, uint32_t alignment, const std::nothrow_t&) noexcept{
#ifdef DEBUG
    std::cout << "operator delete[](void* ptr, const std::nothrow_t&)" << std::endl;
#endif
    sablin::MemoryBase::Free(ptr);
}
#endif
