#include "TCSystemMalloc.h"
#include "TCCommon.h"
#include "PlatformMemory.h"
namespace sablin{

std::mutex TCSystemMalloc::system_malloc_lock_;

std::pair<void*, std::size_t> TCSystemMalloc::Malloc(std::size_t size, std::size_t alignment){
    alignment = Max<std::size_t>(alignment, kPageSize);
    if(size + alignment < size) return {nullptr, 0};

    std::size_t actual_size = RoundUp(size, kMinSystemMalloc);
    if(actual_size < size) return {nullptr, 0};
    alignment = Max<std::size_t>(alignment, kPageSize);

    TCSystemMalloc::system_malloc_lock_.lock();
    auto [result, result_size] = PlatformMemory::BaseMalloc(actual_size, alignment);
    TCSystemMalloc::system_malloc_lock_.unlock();
    return {result, result_size};
}

void TCSystemMalloc::Release(void* ptr, std::size_t size){
#ifdef DEBUG
    ASSERT_WITH_STRING(size % kPageSize == 0, "TCSystemMalloc::Release: Size Can Not Divided By kPageSize!")
#endif
    TCSystemMalloc::system_malloc_lock_.lock();
    PlatformMemory::BaseFree(ptr, size);
    TCSystemMalloc::system_malloc_lock_.unlock();
}
}
