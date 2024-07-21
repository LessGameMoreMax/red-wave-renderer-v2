#ifndef TC_SYSTEM_MALLOC_H
#define TC_SYSTEM_MALLOC_H
#include <utility>
#include <mutex>
#include "Math/MathTools.h"
#include "TCCommon.h"
namespace sablin{

class TCSystemMalloc{
public:
    static std::mutex system_malloc_lock_;
public:
    static std::pair<void*, std::size_t> Malloc(std::size_t size, std::size_t alignment);

    static void Release(void* ptr, std::size_t size);
};

}
#endif
