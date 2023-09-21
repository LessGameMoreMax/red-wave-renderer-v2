#include "TCSystemMalloc.h"
#include "../Math/MathTools.h"
#include "TCCommon.h"
#include "PlatformMemory.h"
#include "../Math/MathTools.h"
namespace sablin{

std::pair<void*, std::size_t> TCSystemMalloc::Malloc(std::size_t size, std::size_t alignment){
    alignment = Max<std::size_t>(alignment, kPageSize);
    if(size + alignment < size) return {nullptr, 0};

    std::size_t actual_size = RoundUp(size, kMinSystemMalloc);
    if(actual_size < size) return {nullptr, 0};
    alignment = Max<std::size_t>(alignment, kPageSize);

    system_malloc_lock_.lock();
    auto [result, result_size] = PlatformMemory::BaseMalloc(size, alignment);
    system_malloc_lock_.unlock();
    return {result, result_size};
}

void TCSystemMalloc::Release(void* ptr, std::size_t size){
#ifdef DEBUG
    ASSERT_WITH_STRING(size % kPageSize == 0, "TCSystemMalloc::Release: Size Can Not Divided By kPageSize!")
#endif
    system_malloc_lock_.lock();
    PlatformMemory::BaseFree(ptr, size);
    system_malloc_lock_.lock();
}
}
