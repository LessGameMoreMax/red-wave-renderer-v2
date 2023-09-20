#include "TCSystemMalloc.h"
#include "../Math/MathTools.h"
#include "TCCommon.h"
#include "PlatformMemory.h"
#include "../Math/MathTools.h"
namespace sablin{

std::pair<void*, std::size_t> TCSystemMalloc::Malloc(std::size_t size, std::size_t alignment){
    alignment = Max<std::size_t>(alignment, kPageSize);
    if(size + alignment < size) return {nullptr, 0};

    system_malloc_lock_.lock();
    auto [result, actual_size] = SystemMalloc(size, alignment);
    system_malloc_lock_.unlock();
    return {result, actual_size};
}

void TCSystemMalloc::Release(void* ptr, std::size_t){

}

//TODO: mmap Just Linux and mmap need test and need better abstract!
std::pair<void*, std::size_t> TCSystemMalloc::SystemMalloc(std::size_t size, std::size_t alignment){
    std::size_t actual_size = RoundUp(size, kMinSystemMalloc);
    if(actual_size < size) return {nullptr, 0};
    alignment = Max<std::size_t>(alignment, Max<std::size_t>(kPageSize, kMinSystemMalloc));
}
}
